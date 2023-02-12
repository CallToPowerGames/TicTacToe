/**
 * TicTacToe
 *
 * Copyright 2014 Denis Meyer, CallToPower Software
 */
#include <QtNetwork>
#include "client.h"
#include "connection.h"
#include "peermanager.h"

Client::Client()
{
    peerManager = new PeerManager(this);
    peerManager->setServerPort(server.serverPort());
    peerManager->startBroadcasting();

    QObject::connect(peerManager, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
    QObject::connect(&server, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
}

void Client::sendMessage(const QString &message)
{
    if (message.isEmpty()) {
        return;
    }

    QList<Connection *> connections = peers.values();
    foreach (Connection *connection, connections) {
        connection->sendMessage(message);
    }
}

QString Client::nickName() const
{
    return QString(peerManager->userName() + " (you)"); // QString(peerManager->userName()) + '@' + QHostInfo::localHostName() + ':' + QString::number(server.serverPort());
}

int Client::getPort() const
{
    return server.serverPort();
}

QString Client::getIPAddress() const
{
    QString ret("");
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    for(int nIter=0; nIter<list.count(); nIter++) {
        if(!list[nIter].isLoopback()) {
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol) {
                ret = list[nIter].toString();
            }
        }

    }
    if(ret.isEmpty()) {
        ret = QHostInfo::localHostName();
    }
    return ret;
}

bool Client::hasConnection(const QHostAddress &senderIp, int senderPort) const
{
    if (senderPort == -1) {
        return peers.contains(senderIp);
    }

    if (!peers.contains(senderIp)) {
        return false;
    }

    QList<Connection *> connections = peers.values(senderIp);
    foreach (Connection *connection, connections) {
        if (connection->peerPort() == senderPort) {
            return true;
        }
    }

    return false;
}

void Client::newConnection(Connection *connection)
{
    connection->setGreetingMessage(peerManager->userName());

    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
}

void Client::challenge(QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendChallenge();
                    break;
                }
            }
        }
    }
}

void Client::giveUp(QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendGiveUp();
                    break;
                }
            }
        }
    }
}

void Client::challengeAccepted(QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendChallengeAccepted();
                    break;
                }
            }
        }
    }
}

void Client::challengeRejected(QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendChallengeRejected();
                    break;
                }
            }
        }
    }
}

void Client::alreadyInGame(QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendAlreadyInGame();
                    break;
                }
            }
        }
    }
}

void Client::challengeOk(QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendOk();
                    break;
                }
            }
        }
    }
}

void Client::challengeNotOk(QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendNotOk();
                    break;
                }
            }
        }
    }
}

void Client::pressedGrid00(const QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendPressedGrid00();
                    break;
                }
            }
        }
    }
}

void Client::pressedGrid01(const QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendPressedGrid01();
                    break;
                }
            }
        }
    }
}

void Client::pressedGrid02(const QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendPressedGrid02();
                    break;
                }
            }
        }
    }
}

void Client::pressedGrid10(const QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendPressedGrid10();
                    break;
                }
            }
        }
    }
}

void Client::pressedGrid11(const QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendPressedGrid11();
                    break;
                }
            }
        }
    }
}

void Client::pressedGrid12(const QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendPressedGrid12();
                    break;
                }
            }
        }
    }
}

void Client::pressedGrid20(const QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendPressedGrid20();
                    break;
                }
            }
        }
    }
}

void Client::pressedGrid21(const QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendPressedGrid21();
                    break;
                }
            }
        }
    }
}

void Client::pressedGrid22(const QString to)
{
    if(to.contains(":")) {
        QStringList l = to.split(":");
        if(l.length() == 2) {
            QHostAddress qaddr = QHostAddress(l.at(0));
            int port = l.at(1).toInt();
            QList<Connection *> connections = peers.values(qaddr);
            foreach (Connection *connection, connections) {
                if(((qaddr.toString().compare(connection->peerAddress().toString())) == 0) && (port == connection->peerPort())) {
                    connection->sendPressedGrid22();
                    break;
                }
            }
        }
    }
}

void Client::readyForUse()
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->peerAddress(), connection->peerPort())) {
        return;
    }

    connect(connection, SIGNAL(newMessage(QString,QString)), this, SIGNAL(newMessage(QString,QString)));
    connect(connection, SIGNAL(newChallenge(QString)), this, SIGNAL(newChallenge(QString)));
    connect(connection, SIGNAL(requestedChallengeAccepted(QString)), this, SIGNAL(gotChallengeAccepted(QString)));
    connect(connection, SIGNAL(requestedChallengeRejected(QString)), this, SIGNAL(gotChallengeRejected(QString)));
    connect(connection, SIGNAL(requestedAlreadyInGame(QString)), this, SIGNAL(gotAlreadyInGame(QString)));
    connect(connection, SIGNAL(requestedChallengeOk(QString)), this, SIGNAL(gotChallengeOk(QString)));
    connect(connection, SIGNAL(requestedChallengeNotOk(QString)), this, SIGNAL(gotChallengeNotOk(QString)));
    connect(connection, SIGNAL(newGiveUp(QString)), this, SIGNAL(newGiveUp(QString)));
    connect(connection, SIGNAL(pressedGrid00(QString)), this, SIGNAL(newPressedGrid00(QString)));
    connect(connection, SIGNAL(pressedGrid01(QString)), this, SIGNAL(newPressedGrid01(QString)));
    connect(connection, SIGNAL(pressedGrid02(QString)), this, SIGNAL(newPressedGrid02(QString)));
    connect(connection, SIGNAL(pressedGrid10(QString)), this, SIGNAL(newPressedGrid10(QString)));
    connect(connection, SIGNAL(pressedGrid11(QString)), this, SIGNAL(newPressedGrid11(QString)));
    connect(connection, SIGNAL(pressedGrid12(QString)), this, SIGNAL(newPressedGrid12(QString)));
    connect(connection, SIGNAL(pressedGrid20(QString)), this, SIGNAL(newPressedGrid20(QString)));
    connect(connection, SIGNAL(pressedGrid21(QString)), this, SIGNAL(newPressedGrid21(QString)));
    connect(connection, SIGNAL(pressedGrid22(QString)), this, SIGNAL(newPressedGrid22(QString)));

    peers.insert(connection->peerAddress(), connection);
    QString nick = connection->name();
    if (!nick.isEmpty()) {
        emit newParticipant(nick);
    }
}

void Client::disconnected()
{
    if (Connection *connection = qobject_cast<Connection *>(sender())) {
        removeConnection(connection);
    }
}

void Client::connectionError(QAbstractSocket::SocketError /* socketError */)
{
    if (Connection *connection = qobject_cast<Connection *>(sender())) {
        removeConnection(connection);
    }
}

void Client::removeConnection(Connection *connection)
{
    if (peers.contains(connection->peerAddress())) {
        peers.remove(connection->peerAddress());
        QString nick = connection->name();
        if (!nick.isEmpty()) {
            emit participantLeft(nick);
        }
    }
    connection->deleteLater();
}
