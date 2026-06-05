#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QNetworkAccessManager>

#include "AuthErrors.h"
#include "SecretStorage.h"

/**
 * Just tested to get authenticated
 */
class Authenticator : public QObject {
  Q_OBJECT
public:
  explicit Authenticator(QObject *parent = nullptr);
  ~Authenticator() override;

  static void startAuth(); // move to slot later
  void saveApiToken(const QString &token);
  void getUser(const QString &token);

signals:
  void finishedAuth(AuthErrors::Errors success);
  void userFetchFinished(bool success);

private slots:
  void onUserFetchFinished(bool ok);

private:
  static constexpr int CLIENT_ID = 28840;
  static constexpr auto AUTH_URL = "https://anilist.co/api/v2/oauth/authorize?client_id=28840&response_type=token";
  static constexpr auto API_URL = "https://graphql.anilist.co";

  SecretStorage m_secretStorage;
  QNetworkAccessManager *m_networkAccessManager;
  QString m_username;
  QString m_pendingToken;
  int m_userId = 0;
};