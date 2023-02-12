/**
 * TicTacToe
 *
 * Copyright 2014 Denis Meyer, CallToPower Software
 */
#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>

#include "server.h"

class PeerManager;

class Client : public QObject
{
    Q_OBJECT

public:
    Client();

    void sendMessage(const QString &message);
    QString nickName() const;
    QString getIPAddress() const;
    int getPort() const;
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;

signals:
    void newMessage(const QString &from, const QString &message);
    void newChallenge(const QString &from);
    void gotChallengeAccepted(QString to);
    void gotChallengeRejected(QString to);
    void gotAlreadyInGame(QString to);
    void gotChallengeOk(QString to);
    void gotChallengeNotOk(QString to);
    void newGiveUp(const QString &from);
    void newPressedGrid00(const QString &from);
    void newPressedGrid01(const QString &from);
    void newPressedGrid02(const QString &from);
    void newPressedGrid10(const QString &from);
    void newPressedGrid11(const QString &from);
    void newPressedGrid12(const QString &from);
    void newPressedGrid20(const QString &from);
    void newPressedGrid21(const QString &from);
    void newPressedGrid22(const QString &from);
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);

private slots:
    void newConnection(Connection *connection);
    void connectionError(QAbstractSocket::SocketError socketError);
    void disconnected();
    void readyForUse();
    void challenge(QString to);
    void giveUp(QString to);
    void challengeAccepted(QString to);
    void challengeRejected(QString to);
    void alreadyInGame(const QString to);
    void challengeOk(const QString to);
    void challengeNotOk(const QString to);
    void pressedGrid00(const QString to);
    void pressedGrid01(const QString to);
    void pressedGrid02(const QString to);
    void pressedGrid10(const QString to);
    void pressedGrid11(const QString to);
    void pressedGrid12(const QString to);
    void pressedGrid20(const QString to);
    void pressedGrid21(const QString to);
    void pressedGrid22(const QString to);

private:
    void removeConnection(Connection *connection);

    PeerManager *peerManager;
    Server server;
    QMultiHash<QHostAddress, Connection *> peers;
};

#endif // CLIENT_H
