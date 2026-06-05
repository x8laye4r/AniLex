#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "AuthErrors.h"

class AuthServer : public QObject {
  Q_OBJECT
public:
  explicit AuthServer(QObject *parent = nullptr);
  ~AuthServer() override;

signals:
  void tokenReceived(const QString &token);

public slots:
  void onAuthResult(AuthErrors::Errors success);

private:
  static constexpr int PORT = 55000;

  void handleRequest(QTcpSocket *socket);

  QTcpServer m_server;
  QTcpSocket *m_pendingSocket = nullptr;
};
