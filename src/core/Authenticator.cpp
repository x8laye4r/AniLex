#include "anilex/core/Authenticator.h"

#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QTcpSocket>
#include <QDesktopServices>
#include <QJsonObject>
#include <QNetworkReply>

#include "anilex/utils/AppPaths.h"

static void sendResponse(QTcpSocket *socket, const QByteArray &content, const QByteArray &contentType) {
    QByteArray response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: " + contentType + "\r\n";
    response += "Content-Length: " + QByteArray::number(content.size()) + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += content;

    socket->write(response);
    socket->disconnectFromHost();
}

static QString readApiToken(const QByteArray &requestLine) {
    QList<QByteArray> parts = requestLine.split(' ');
    if (parts.size() < 2) return QString{};

    QUrl url = QUrl::fromEncoded(parts[1]);
    QUrlQuery query(url.query());
    QString token = query.queryItemValue("access_token");
    return token;
}

void Authenticator::onUserFetchFinished(bool ok) {
    if (!ok) {
        qCritical() << "Failed to fetch user";
        emit finishedAuth(false);
        return;
    }
    m_keyring.saveSecret("auth", m_pendingToken);
}

Authenticator::Authenticator() {
    m_manager = new QNetworkAccessManager(this);

    connect(this, &Authenticator::userFetchFinished, this, &Authenticator::onUserFetchFinished);

    connect(&m_keyring, &SecretStorage::secretStored, this, [this](const QString &key) {
        if (key == "auth") {
            emit readyToSendResponse(true);
            emit finishedAuth(true);
        }
    });


    connect(&m_server, &QTcpServer::newConnection, this, [this]() {
        QTcpSocket *socket = m_server.nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, [this, socket] {
            handleRequest(socket);
        });
    });

    m_server.listen(QHostAddress::LocalHost, port);
    qInfo() << "Server runs on http://localhost:55000";
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
        qWarning() << "Could not open GraphQL file" << file.errorString();
        emit userFetchFinished(false);
        return;
    }
    payload["query"] = QString::fromUtf8(file.readAll());
    file.close();

    QNetworkReply *reply = m_manager->post(request, QJsonDocument(payload).toJson());

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        const QByteArray body = reply->readAll();

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "HTTP/Network Error" << reply->errorString();
            reply->deleteLater();
            emit userFetchFinished(false);
            return;
        }

        QJsonParseError pe{};
        const QJsonDocument doc = QJsonDocument::fromJson(body, &pe);
        if (pe.error != QJsonParseError::NoError || !doc.isObject()) {
            qWarning () << "JSON parse error:" << pe.errorString();
            reply->deleteLater();
            emit userFetchFinished(false);
            return;
        }
        const QJsonObject root = doc.object();

        if (root.contains("errors")) {
            qWarning() << "GraphQL errors detected:" << root.value("errors");
            reply->deleteLater();
            emit userFetchFinished(false);
            return;
        }

        this->m_username =
            root.value("data").toObject()
                    .value("Viewer").toObject()
                    .value("name").toString();
        this->m_userId =
            root.value("data").toObject()
                    .value("Viewer").toObject()
                    .value("id").toInt();
        qInfo() << "username:" << this->m_username << "id:" << this->m_userId;
        reply->deleteLater();

        QJsonObject json;
        json["username"] = this->m_username;
        json["id"] = this->m_userId;
        json["token_expires_at"] = QDateTime::currentDateTime().addDays(365).toString();

        QFile file(AppPaths::appDataPath() + "/user.json");
        qInfo() << file.fileName();
        QJsonDocument document(json);

        if (!file.open(QIODevice::WriteOnly)) {
            qInfo() << "Couldn't open file" << file.errorString();
            emit userFetchFinished(false);
            return;
        }
        file.write(document.toJson());
        file.close();
        emit userFetchFinished(true);
    });
}

void Authenticator::saveApiToken(const QString &token) {
    if (token.isEmpty()) {
        qCritical() << "Access token is empty, cannot save";
        emit finishedAuth(false);
        return;
    }
    m_pendingToken = token;
    getUser(token);
}

void Authenticator::handleRequest(QTcpSocket *socket) {
    QByteArray data = socket->readAll();

    QString request_line = QString::fromUtf8(data.split('\n').first()).trimmed();

    if (request_line.startsWith("GET /token")) {
        QByteArray lineData = request_line.toUtf8();
        QString token = readApiToken(lineData);
        saveApiToken(token);
        connect(this, &Authenticator::readyToSendResponse, this, [socket](bool success) {
            if (success) {
                sendResponse(socket, "<p>OK</p>", "text/html; charset=utf-8");
            } else {
                QByteArray response = "HTTP/1.1 400 Bad Request\r\n"
                                      "Content-Type: text/html\r\n"
                                      "Connection: close\r\n\r\n"
                                      "<p>Auth Failed</p>";
                if (socket->isOpen()) {
                    socket->write(response);
                    socket->disconnectFromHost();
                }
            }
        }, Qt::SingleShotConnection); // single shot, because it only needs to be run once
        return;
    }

    QString filePath = ":/assets/index.html";
    QString contentType = "text/html";

    qInfo() << "Request line:" << request_line;

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
    m_server.close();
    qInfo() << "Server Closed";
}

void Authenticator::startAuth() {
    QDesktopServices::openUrl(QUrl(auth_url));
}