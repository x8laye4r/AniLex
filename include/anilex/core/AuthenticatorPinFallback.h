#pragma once
#include <QNetworkAccessManager>
#include <QObject>

#include "AuthErrors.h"
#include "SecretStorage.h"

class AuthenticatorPinFallback : public QObject {
  Q_OBJECT
public:
  explicit AuthenticatorPinFallback(QObject* parent = nullptr);

signals:
  void finishedAuth(AuthErrors::Errors success);
  void userFetchFinished(bool success);

public slots:
  void fetchUser(const QString &token);
  void startAuth();

private slots:
  void onUserFetchFinished(bool ok);

private:
  static constexpr int CLIENT_ID = 42850;
  static constexpr auto URL = "https://anilist.co/api/v2/oauth/authorize?client_id=42850&response_type=token";
  static constexpr auto API_URL = "https://graphql.anilist.co";

  void getUser(const QString &token);

  SecretStorage *m_secretStorage = nullptr;
  QNetworkAccessManager m_networkAccessManager;
  QString m_pendingToken;
};
