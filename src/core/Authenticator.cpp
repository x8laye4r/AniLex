#include "anilex/core/Authenticator.h"

Authenticator::Authenticator() {
    connect(&server, &QTcpServer::newConnection, this, [this]() {
        QTcpSocket* socket = server.nextPendingConnection();

        connect(socket, &QTcpSocket::readyRead, socket, [socket]() {
            QByteArray request = socket->readAll();
            qDebug() << request;

            QByteArray response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "\r\n"
                "<html><body>Server läuft</body></html>";

            socket->write(response);
            socket->disconnectFromHost();
        });
    });

    server.listen(QHostAddress::LocalHost, this->port);
    qInfo() << "Server läuft auf http://localhost:17654";
}

Authenticator::~Authenticator() {
    server.close();
    qInfo() << "Server Closed";
}

void Authenticator::startAuth() {
}
