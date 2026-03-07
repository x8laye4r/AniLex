#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QNetworkAccessManager>
#include "anilex/core/SecretStorage.h"

class Authenticator : public QObject {
    Q_OBJECT
private:
    static constexpr int clientId = 28840;
    static constexpr int port = 55000;
    static constexpr auto auth_url = "https://anilist.co/api/v2/oauth/authorize?client_id=28840&response_type=token";
    static constexpr auto api_url = "https://graphql.anilist.co";
    SecretStorage keyring;
    QNetworkAccessManager *manager;
    QString username;
    int userId;
public:
    Authenticator();
    ~Authenticator();

    QTcpServer server;

    void startAuth();
    void handleRequest(QTcpSocket *socket);
    void saveApiToken(const QString &token);
    void getUser(const QString &token);

signals:
    void finishedAuth(bool ok);
};