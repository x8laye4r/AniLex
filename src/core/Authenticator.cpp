#include "anilex/core/Authenticator.h"

#include <QUrl>
#include <QFile>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "anilex/utils/AppPaths.h"

Authenticator::Authenticator(QObject *parent)
  : QObject(parent) {
  m_networkAccessManager = new QNetworkAccessManager(this);

  connect(this, &Authenticator::userFetchFinished, this, &Authenticator::onUserFetchFinished);

  connect(&m_secretStorage, &SecretStorage::secretStored, this, [this](const QString &key) {
    if (key == "auth") {
      emit finishedAuth(true);
    }
  });
}

Authenticator::~Authenticator() = default;

void Authenticator::startAuth() {
  QDesktopServices::openUrl(QUrl(AUTH_URL));
}

void Authenticator::saveApiToken(const QString &token) {
  if (token.isEmpty()) {
    qCritical() << "Access Token is empty, cannot save.";
    emit finishedAuth(false);
    return;
  }
  m_pendingToken = token;
  getUser(token);
}

void Authenticator::onUserFetchFinished(bool ok) {
  if (!ok) {
    qCritical() << "Failed to fetch user";
    emit finishedAuth(false);
    return;
  }
  m_secretStorage.saveSecret("auth", m_pendingToken);
}

void Authenticator::getUser(const QString &token) {
    QNetworkRequest request;
    request.setUrl(QUrl(API_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Authorization", QByteArray("Bearer ") + token.toUtf8());

    QFile file(":/graphql/viewer/GetViewerQuery.graphql");
    QJsonObject payload;
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open GraphQL file:" << file.errorString();
        emit userFetchFinished(false);
        return;
    }
    payload["query"] = QString::fromUtf8(file.readAll());
    file.close();

    QNetworkReply *reply = m_networkAccessManager->post(request, QJsonDocument(payload).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const QByteArray body = reply->readAll();

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "HTTP/Network error:" << reply->errorString();
            reply->deleteLater();
            emit userFetchFinished(false);
            return;
        }

        QJsonParseError pe{};
        const QJsonDocument doc = QJsonDocument::fromJson(body, &pe);
        if (pe.error != QJsonParseError::NoError || !doc.isObject()) {
            qWarning() << "JSON parse error:" << pe.errorString();
            reply->deleteLater();
            emit userFetchFinished(false);
            return;
        }

        const QJsonObject root = doc.object();
        if (root.contains("errors")) {
            qWarning() << "GraphQL errors:" << root.value("errors");
            reply->deleteLater();
            emit userFetchFinished(false);
            return;
        }

        const QJsonObject viewer =
            root.value("data").toObject()
                .value("Viewer").toObject();

        m_username = viewer.value("name").toString();
        m_userId   = viewer.value("id").toInt();
        qInfo() << "username:" << m_username << "id:" << m_userId;
        reply->deleteLater();

        QJsonObject json;
        json["username"]         = m_username;
        json["id"]               = m_userId;
        json["token_expires_at"] = QDateTime::currentDateTime().addDays(365).toString();

        QFile outFile(AppPaths::appDataPath() + "/user.json");
        qInfo() << outFile.fileName();
        if (!outFile.open(QIODevice::WriteOnly)) {
            qWarning() << "Couldn't open file:" << outFile.errorString();
            emit userFetchFinished(false);
            return;
        }
        outFile.write(QJsonDocument(json).toJson());
        outFile.close();

        emit userFetchFinished(true);
    });
}
