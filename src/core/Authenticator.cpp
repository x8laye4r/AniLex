#include "anilex/core/Authenticator.h"

#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QTcpSocket>
#include <QDesktopServices>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkReply>

#include "anilex/ui/MainWindow.h"
#include "anilex/utils/AppPaths.h"

static void sendResponse(QTcpSocket *socket, const QByteArray &content, const QByteArray &contentType) {
    QByteArray response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: " + contentType + "\r\n";
    response += "Content-Length: " + QByteArray::number(content.size()) + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += content;

    socket->write(response);
    socket->waitForBytesWritten();
    socket->disconnectFromHost();
}

[[nodiscard]] static QString readApiToken(QByteArray &requestLine) {
    QList<QByteArray> parts = requestLine.split(' ');
    if (parts.size() < 2) return QString{};

    QUrl url = QUrl::fromEncoded(parts[1]);
    QUrlQuery query(url.query());
    QString token = query.queryItemValue("access_token");
    if (!token.isEmpty()) {
        qInfo() << "Token received:" << token;
    }
    return token;
}

Authenticator::Authenticator() {
    manager = new QNetworkAccessManager(this);
    connect(&server, &QTcpServer::newConnection, this, [this]() {
        QTcpSocket *socket = server.nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, [this, socket] {
           handleRequest(socket);
        });
    });

    server.listen(QHostAddress::LocalHost, port);
    qInfo() << "Server läuft auf http://localhost:55000";
}

void Authenticator::getUser(const QString &token) {
    QNetworkRequest request;
    request.setUrl(QUrl(api_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Authorization", QByteArray("Bearer ") + token.toUtf8());

    QFile file(":/graphql/viewer/GetViewerQuery.graphql");
    QJsonObject payload;
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could open GraphQL file" << file.errorString();
        return;
    }
    payload["query"] = QString::fromUtf8(file.readAll());
    file.close();

    QNetworkReply *reply = manager->post(request, QJsonDocument(payload).toJson());

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        const QByteArray body = reply->readAll();

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "HTTP/Network Error" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QJsonParseError pe{};
        const QJsonDocument doc = QJsonDocument::fromJson(body, &pe);
        if (pe.error != QJsonParseError::NoError || !doc.isObject()) {
            qWarning () << "JSON parse error:" << pe.errorString();
            reply->deleteLater();
            return;
        }
        const QJsonObject root = doc.object();

        if (root.contains("errors")) {
             qWarning() << "GraphQL errors detected:" << root.value("errors");
             reply->deleteLater();
             return;
        }

        this->username =
            root.value("data").toObject()
                    .value("Viewer").toObject()
                    .value("name").toString();
        this->userId =
            root.value("data").toObject()
                    .value("Viewer").toObject()
                    .value("id").toInt();
        qInfo() << "username:" << this->username << "id:" << this->userId;
        reply->deleteLater();

        QJsonObject json;
        json["username"] = this->username;
        json["id"] = this->userId;
        json["update_token"] = QDateTime::currentDateTime().addDays(365).toString();

        QFile file(AppPaths::appDataPath() + "/user.json");
        qInfo() << file.fileName();
        QJsonDocument document(json);

        if (!file.open(QIODevice::WriteOnly)) {
            qInfo() << "Couldn't open file" << file.errorString();
            return;
        }
        file.write(document.toJson());
        file.close();
    });
}

void Authenticator::saveApiToken(const QString &token) {
    this->getUser(token);
    keyring.saveSecret("auth", token);
}

void Authenticator::handleRequest(QTcpSocket *socket) {
    QByteArray data = socket->readAll();

    QString request_line = QString::fromUtf8(data.split('\n').first()).trimmed();

    if (request_line.startsWith("GET /token")) {
        QByteArray lineData = request_line.toUtf8();
        QString token = readApiToken(lineData);
        saveApiToken(token);
        sendResponse(socket, "<p>OK<p>", "text/html; charset=utf-8");
        return;
    }

    QString filePath = ":/assets/index.html";
    QString contentType = "text/html";

    if (request_line.contains("/styles.css")) {
        filePath = ":/assets/styles.css";
        contentType = "text/css";
    }
    else if (request_line.contains("/script.js")) {
        filePath = ":/assets/script.js";
        contentType = "text/javascript";
    }
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        sendResponse(socket, file.readAll(), contentType.toUtf8());
    } else {
        qInfo() << "Failed to open file" << filePath << ":" << file.errorString();
        socket->disconnectFromHost();
    }
}

Authenticator::~Authenticator() {
    server.close();
    qInfo() << "Server Closed";
}

void Authenticator::startAuth() {
    QDesktopServices::openUrl(QUrl(auth_url));
}