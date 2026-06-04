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

private:
  static constexpr int PORT = 55000;

  QTcpServer m_server;
  QTcpSocket *m_pendingSocket = nullptr;

  void handleRequest(QTcpSocket *socket);

public slots:
  void onAuthResult(AuthErrors::Errors success);

signals:
  void tokenReceived(const QString &token);
};
