/**
 * TicTacToe
 *
 * Copyright 2014 Denis Meyer, CallToPower Software
 */
#include "connection.h"

#include <QtNetwork>

static const int TransferTimeout = 30 * 1000;
static const int PongTimeout = 60 * 1000;
static const int PingInterval = 5 * 1000;
static const char SeparatorToken = ' ';

Connection::Connection(QObject *parent)
    : QTcpSocket(parent)
{
    greetingMessage = tr("undefined");
    username = tr("unknown");
    state = WaitingForGreeting;
    currentDataType = Undefined;
    numBytesForCurrentDataType = -1;
    transferTimerId = 0;
    isGreetingMessageSent = false;
    pingTimer.setInterval(PingInterval);

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
    QObject::connect(this, SIGNAL(disconnected()), &pingTimer, SLOT(stop()));
    QObject::connect(&pingTimer, SIGNAL(timeout()), this, SLOT(sendPing()));
    QObject::connect(this, SIGNAL(connected()), this, SLOT(sendGreetingMessage()));
}

QString Connection::name() const
{
    return username;
}

void Connection::setGreetingMessage(const QString &message)
{
    greetingMessage = message;
}

bool Connection::sendMessage(const QString &message)
{
    if (message.isEmpty()) {
        return false;
    }

    QByteArray msg = message.toUtf8();
    QByteArray data = "MESSAGE " + QByteArray::number(msg.size()) + " " + msg;
    return write(data) == data.size();
}

void Connection::timerEvent(QTimerEvent *timerEvent)
{
    if (timerEvent->timerId() == transferTimerId) {
        abort();
        killTimer(transferTimerId);
        transferTimerId = 0;
    }
}

void Connection::processReadyRead()
{
    if (state == WaitingForGreeting) {
        if (!readProtocolHeader()) {
            return;
        }
        if (currentDataType != Greeting) {
            abort();
            return;
        }
        state = ReadingGreeting;
    }

    if (state == ReadingGreeting) {
        if (!hasEnoughData())
            return;

        buffer = read(numBytesForCurrentDataType);
        if (buffer.size() != numBytesForCurrentDataType) {
            abort();
            return;
        }

        username = QString(buffer)
                + '@'
                + peerAddress().toString()
                + ':'
                + QString::number(peerPort());
        currentDataType = Undefined;
        numBytesForCurrentDataType = 0;
        buffer.clear();

        if (!isValid()) {
            abort();
            return;
        }

        if (!isGreetingMessageSent) {
            sendGreetingMessage();
        }

        pingTimer.start();
        pongTime.start();
        state = ReadyForUse;
        emit readyForUse();
    }

    do {
        if (currentDataType == Undefined) {
            if (!readProtocolHeader())
                return;
        }
        if (!hasEnoughData()) {
            return;
        }
        processData();
    } while (bytesAvailable() > 0);
}

void Connection::sendGreetingMessage()
{
    QByteArray greeting = greetingMessage.toUtf8();
    QByteArray data = "GREETING " + QByteArray::number(greeting.size()) + ' ' + greeting;
    if (write(data) == data.size()) {
        isGreetingMessageSent = true;
    }
}

void Connection::sendPing()
{
    if (pongTime.elapsed() > PongTimeout) {
        abort();
        return;
    }

    write("PING 1 p");
}

void Connection::sendChallenge()
{
    write("CHALLENGE 1 p");
}

void Connection::sendGiveUp()
{
    write("GIVEUP 1 p");
}

void Connection::sendChallengeAccepted()
{
    write("ACCEPTED 1 p");
}

void Connection::sendChallengeRejected()
{
    write("REJECTED 1 p");
}

void Connection::sendAlreadyInGame()
{
    write("ALREADYINGAME 1 p");
}

void Connection::sendOk()
{
    write("OK 1 p");
}

void Connection::sendNotOk()
{
    write("NOTOK 1 p");
}

void Connection::sendPressedGrid00()
{
    write("PRESSEDGRID00 1 p");
}

void Connection::sendPressedGrid01()
{
    write("PRESSEDGRID01 1 p");
}

void Connection::sendPressedGrid02()
{
    write("PRESSEDGRID02 1 p");
}

void Connection::sendPressedGrid10()
{
    write("PRESSEDGRID10 1 p");
}

void Connection::sendPressedGrid11()
{
    write("PRESSEDGRID11 1 p");
}

void Connection::sendPressedGrid12()
{
    write("PRESSEDGRID12 1 p");
}

void Connection::sendPressedGrid20()
{
    write("PRESSEDGRID20 1 p");
}

void Connection::sendPressedGrid21()
{
    write("PRESSEDGRID21 1 p");
}

void Connection::sendPressedGrid22()
{
    write("PRESSEDGRID22 1 p");
}

int Connection::readDataIntoBuffer(int maxSize)
{
    if (maxSize > MaxBufferSize) {
        return 0;
    }

    int numBytesBeforeRead = buffer.size();
    if (numBytesBeforeRead == MaxBufferSize) {
        abort();
        return 0;
    }

    while ((bytesAvailable() > 0) && (buffer.size() < maxSize)) {
        buffer.append(read(1));
        if (buffer.endsWith(SeparatorToken)) {
            break;
        }
    }
    return buffer.size() - numBytesBeforeRead;
}

int Connection::dataLengthForCurrentDataType()
{
    if ((bytesAvailable() <= 0) || (readDataIntoBuffer() <= 0) || !buffer.endsWith(SeparatorToken)) {
        return 0;
    }

    buffer.chop(1);
    int number = buffer.toInt();
    buffer.clear();
    return number;
}

bool Connection::readProtocolHeader()
{
    if (transferTimerId) {
        killTimer(transferTimerId);
        transferTimerId = 0;
    }

    if (readDataIntoBuffer() <= 0) {
        transferTimerId = startTimer(TransferTimeout);
        return false;
    }

    if (buffer == "PING ") {
        currentDataType = Ping;
    } else if (buffer == "PONG ") {
        currentDataType = Pong;
    } else if (buffer == "MESSAGE ") {
        currentDataType = PlainText;
    } else if (buffer == "GREETING ") {
        currentDataType = Greeting;
    } else if (buffer == "CHALLENGE ") {
        currentDataType = Challenge;
    } else if (buffer == "ACCEPTED ") {
        currentDataType = ChallengeAccepted;
    } else if (buffer == "REJECTED ") {
        currentDataType = ChallengeRejected;
    } else if (buffer == "ALREADYINGAME ") {
        currentDataType = AlreadyInGame;
    } else if (buffer == "OK ") {
        currentDataType = ChallengeOk;
    } else if (buffer == "NOTOK ") {
        currentDataType = ChallengeNotOk;
    } else if (buffer == "GIVEUP ") {
        currentDataType = GiveUp;
    } else if (buffer == "PRESSEDGRID00 ") {
        currentDataType = PressedGrid00;
    } else if (buffer == "PRESSEDGRID01 ") {
        currentDataType = PressedGrid01;
    } else if (buffer == "PRESSEDGRID02 ") {
        currentDataType = PressedGrid02;
    } else if (buffer == "PRESSEDGRID10 ") {
        currentDataType = PressedGrid10;
    } else if (buffer == "PRESSEDGRID11 ") {
        currentDataType = PressedGrid11;
    } else if (buffer == "PRESSEDGRID12 ") {
        currentDataType = PressedGrid12;
    } else if (buffer == "PRESSEDGRID20 ") {
        currentDataType = PressedGrid20;
    } else if (buffer == "PRESSEDGRID21 ") {
        currentDataType = PressedGrid21;
    } else if (buffer == "PRESSEDGRID22 ") {
        currentDataType = PressedGrid22;
    } else {
        currentDataType = Undefined;
        abort();
        return false;
    }

    buffer.clear();
    numBytesForCurrentDataType = dataLengthForCurrentDataType();
    return true;
}

bool Connection::hasEnoughData()
{
    if (transferTimerId) {
        QObject::killTimer(transferTimerId);
        transferTimerId = 0;
    }

    if (numBytesForCurrentDataType <= 0) {
        numBytesForCurrentDataType = dataLengthForCurrentDataType();
    }

    if (bytesAvailable() < numBytesForCurrentDataType || numBytesForCurrentDataType <= 0) {
        transferTimerId = startTimer(TransferTimeout);
        return false;
    }

    return true;
}

void Connection::processData()
{
    buffer = read(numBytesForCurrentDataType);
    if (buffer.size() != numBytesForCurrentDataType) {
        abort();
        return;
    }

    switch (currentDataType) {
    case PlainText:
        emit newMessage(username, QString::fromUtf8(buffer));
        break;
    case Ping:
        write("PONG 1 p");
        break;
    case Pong:
        pongTime.restart();
        break;
    case Challenge:
        emit newChallenge(username);
        break;
    case ChallengeAccepted:
        emit requestedChallengeAccepted(username);
        break;
    case ChallengeRejected:
        emit requestedChallengeRejected(username);
        break;
    case AlreadyInGame:
        emit requestedAlreadyInGame(username);
        break;
    case ChallengeOk:
        emit requestedChallengeOk(username);
        break;
    case ChallengeNotOk:
        emit requestedChallengeNotOk(username);
        break;
    case GiveUp:
        emit newGiveUp(username);
        break;
    case PressedGrid00:
        emit pressedGrid00(username);
        break;
    case PressedGrid01:
        emit pressedGrid01(username);
        break;
    case PressedGrid02:
        emit pressedGrid02(username);
        break;
    case PressedGrid10:
        emit pressedGrid10(username);
        break;
    case PressedGrid11:
        emit pressedGrid11(username);
        break;
    case PressedGrid12:
        emit pressedGrid12(username);
        break;
    case PressedGrid20:
        emit pressedGrid20(username);
        break;
    case PressedGrid21:
        emit pressedGrid21(username);
        break;
    case PressedGrid22:
        emit pressedGrid22(username);
        break;
    default:
        break;
    }

    currentDataType = Undefined;
    numBytesForCurrentDataType = 0;
    buffer.clear();
}
