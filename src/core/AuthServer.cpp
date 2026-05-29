#include "anilex/core/AuthServer.h"

#include <QFile>
#include <QUrl>
#include <QUrlQuery>

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
  return query.queryItemValue("access_token");
}

AuthServer::AuthServer(QObject *parent)
  : QObject(parent) {
  connect(&m_server, &QTcpServer::newConnection, this, [this]() {
    QTcpSocket *socket = m_server.nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
      handleRequest(socket);
    });
  });

  m_server.listen(QHostAddress::LocalHost, PORT);
  qInfo() << "Server runs on http://localhost:55000";
}

AuthServer::~AuthServer() {
  m_server.close();
  qInfo() << "Server closed";
}

void AuthServer::handleRequest(QTcpSocket *socket) {
  const QByteArray data = socket->readAll();
  const QString requestLine = QString::fromUtf8(data.split('\n').first()).trimmed();

  qInfo() << "Request line: " << requestLine;

  if (requestLine.startsWith("GET /token")) {
    const QString token = readApiToken(requestLine.toUtf8());
    m_pendingSocket = socket;
    emit tokenReceived(token);
    return;
  }

  QString filePath = ":/index.html";
  QString contentType = "text/html";

  if (requestLine.contains("/styles.css")) {
    filePath = ":/styles.css";
    contentType = "text/css";
  } else if (requestLine.contains("/script.js")) {
    filePath = ":/script.js";
    contentType = "text/javascript";
  }

  QFile file(filePath);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    sendResponse(socket, file.readAll(), contentType.toUtf8());
  } else {
    qWarning() << "Failed to open file" << filePath << ":" << file.errorString();
    socket->disconnectFromHost();
  }
}

void AuthServer::onAuthResult(bool success) {
  if (!m_pendingSocket) return;

  if (success) {
    sendResponse(m_pendingSocket, "<p>OK</p>", "text/html; charset=utf-8");
  } else {
    const QByteArray response =
        "HTTP/1.1 400 Bad Request\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n\r\n"
        "<p>Auth Failed</p>";
    if (m_pendingSocket->isOpen()) {
      m_pendingSocket->write(response);
      m_pendingSocket->disconnectFromHost();
    }
  }

  m_pendingSocket = nullptr;
}

