#ifndef CONNECTION_H
#define CONNECTION_H

/**
 * TicTacToe
 *
 * Copyright 2014 Denis Meyer, CallToPower Software
 */
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

static const int MaxBufferSize = 1024000;

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    enum ConnectionState {
        WaitingForGreeting,
        ReadingGreeting,
        ReadyForUse
    };
    enum DataType {
        PlainText,
        Ping,
        Pong,
        Greeting,
        Challenge,
        ChallengeAccepted,
        ChallengeRejected,
        AlreadyInGame,
        ChallengeOk,
        ChallengeNotOk,
        GiveUp,
        PressedGrid00,
        PressedGrid01,
        PressedGrid02,
        PressedGrid10,
        PressedGrid11,
        PressedGrid12,
        PressedGrid20,
        PressedGrid21,
        PressedGrid22,
        Undefined
    };

    Connection(QObject *parent = 0);

    QString name() const;
    void setGreetingMessage(const QString &message);
    bool sendMessage(const QString &message);
    void sendGiveUp();
    void sendChallenge();
    void sendChallengeAccepted();
    void sendChallengeRejected();
    void sendAlreadyInGame();
    void sendOk();
    void sendNotOk();
    void sendPressedGrid00();
    void sendPressedGrid01();
    void sendPressedGrid02();
    void sendPressedGrid10();
    void sendPressedGrid11();
    void sendPressedGrid12();
    void sendPressedGrid20();
    void sendPressedGrid21();
    void sendPressedGrid22();

signals:
    void readyForUse();
    void newMessage(const QString &from, const QString &message);
    void newChallenge(const QString &from);
    void newGiveUp(const QString &from);
    void requestedChallengeAccepted(const QString &from);
    void requestedChallengeRejected(const QString &from);
    void requestedAlreadyInGame(const QString &from);
    void requestedChallengeOk(const QString &from);
    void requestedChallengeNotOk(const QString &from);
    void pressedGrid00(const QString &from);
    void pressedGrid01(const QString &from);
    void pressedGrid02(const QString &from);
    void pressedGrid10(const QString &from);
    void pressedGrid11(const QString &from);
    void pressedGrid12(const QString &from);
    void pressedGrid20(const QString &from);
    void pressedGrid21(const QString &from);
    void pressedGrid22(const QString &from);

protected:
    void timerEvent(QTimerEvent *timerEvent);

private slots:
    void processReadyRead();
    void sendGreetingMessage();
    void sendPing();

private:
    int readDataIntoBuffer(int maxSize = MaxBufferSize);
    int dataLengthForCurrentDataType();
    bool readProtocolHeader();
    bool hasEnoughData();
    void processData();

    QString greetingMessage;
    QString username;
    QTimer pingTimer;
    QTime pongTime;
    QByteArray buffer;
    ConnectionState state;
    DataType currentDataType;
    int numBytesForCurrentDataType;
    int transferTimerId;
    bool isGreetingMessageSent;
};

#endif // CONNECTION_H
