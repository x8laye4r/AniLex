#pragma once
#include <QNetworkAccessManager>
#include <QObject>

#include "AuthErrors.h"
#include "SecretStorage.h"

class AuthenticatorPinFallback : public QObject {
  Q_OBJECT
public:
  explicit AuthenticatorPinFallback(QObject* parent = nullptr);
private:
  void getUser(const QString &token);

  constexpr static int CLIENT_ID = 42850;
  constexpr static auto URL = "https://anilist.co/api/v2/oauth/authorize?client_id=42850&response_type=token";
  static constexpr auto API_URL = "https://graphql.anilist.co";

  SecretStorage *m_secretStorage = nullptr;
  QNetworkAccessManager m_networkAccessManager;
  QString m_pendingToken = "";

private slots:
  void onUserFetchFinished(bool ok);
public slots:
  void fetchUser(const QString &token);
  void startAuth();

signals:
  void finishedAuth(AuthErrors::Errors success);
  void userFetchFinished(bool success);
};
