#include "anilex/core/AuthenticatorPinFallback.h"

#include <QDesktopServices>
#include <QFile>
#include <QJsonObject>
#include <QNetworkReply>

#include "anilex/utils/AppPaths.h"

AuthenticatorPinFallback::AuthenticatorPinFallback(QObject *parent) {
  m_secretStorage = new SecretStorage(this);

  connect(this, &AuthenticatorPinFallback::userFetchFinished, this, &AuthenticatorPinFallback::onUserFetchFinished);
  connect(m_secretStorage, &SecretStorage::secretStored, this, [this](const QString &key) {
      if (!key.isEmpty() && key == "auth") {
          emit finishedAuth(AuthErrors::Errors::NoError);
      } else {
          emit finishedAuth(AuthErrors::Errors::KeyringSaveError);
      }
  });
}

void AuthenticatorPinFallback::startAuth() {
    QDesktopServices::openUrl(QUrl(URL));
}

void AuthenticatorPinFallback::getUser(const QString &token) {
    QNetworkRequest request;
    request.setUrl(QUrl(API_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Authorization", QByteArray("Bearer ") + token.toUtf8());

    QFile file(":/graphql/viewer/GetViewerQuery.graphql");
    QJsonObject payload;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open GraphQL file:" << file.errorString();
        emit userFetchFinished(false);
        return;
    }
    payload["query"] = QString::fromUtf8(file.readAll());
    file.close();

    QNetworkReply *reply = m_networkAccessManager.post(request, QJsonDocument(payload).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
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

        const QString username = viewer.value("name").toString();
        const qint32 userId = viewer.value("id").toInt();
        qInfo() << "username:" << username << "id:" << userId;

        QJsonObject json;
        json["username"] = username;
        json["id"] = userId;
        json["token_expires_at"] = QDateTime::currentDateTime().addDays(365).toString(Qt::ISODate);

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

void AuthenticatorPinFallback::onUserFetchFinished(const bool ok) {
    if (ok) {
        m_secretStorage->saveSecret("auth", m_pendingToken);
    } else {
        emit finishedAuth(AuthErrors::Errors::UserFetchError);
    }
}

void AuthenticatorPinFallback::fetchUser(const QString &token) {
    m_pendingToken = token;
    getUser(token);
}

