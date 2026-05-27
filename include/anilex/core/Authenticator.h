#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QNetworkAccessManager>
#include "anilex/core/SecretStorage.h"

/**
 * Just tested to get authenticated
 */
class Authenticator : public QObject {
    Q_OBJECT
private:
    static constexpr int clientId = 28840;
    static constexpr int port = 55000;
    static constexpr auto auth_url = "https://anilist.co/api/v2/oauth/authorize?client_id=28840&response_type=token";
    static constexpr auto api_url = "https://graphql.anilist.co";
    SecretStorage m_keyring;
    QNetworkAccessManager *m_manager;
    QString m_username;
    QString m_pendingToken;
    int m_userId;
    QTcpServer m_server;
private slots:
    void onUserFetchFinished(bool ok);
public:
    Authenticator();
    ~Authenticator() override;

    static void startAuth();
    void handleRequest(QTcpSocket *socket);
    void saveApiToken(const QString &token);
    void getUser(const QString &token);

signals:
    void finishedAuth(bool ok);
    void userFetchFinished(bool ok);
    void readyToSendResponse(bool success);
};