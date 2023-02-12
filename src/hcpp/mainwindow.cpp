/**
 * TicTacToe
 *
 * Copyright 2014 Denis Meyer, CallToPower Software
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    about(0), game(0), msgBox(0), img_empty(0), img_x(0), img_o(0), label_statusbar(0),
    challengingPlayerHasFirstMove(true), gameInPreparation(false)
{
    ui->setupUi(this);
    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->textEdit->setReadOnly(true);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);

    img_empty = new QPixmap(":/img/empty");
    img_x = new QPixmap(":/img/x");
    img_o = new QPixmap(":/img/o");

    about = new TextWindow(this);
    about->setTitle("About");
    about->setText(":/text/about");

    list_grid << ui->grid_00 << ui->grid_01 << ui->grid_02;
    list_grid << ui->grid_10 << ui->grid_11 << ui->grid_12;
    list_grid << ui->grid_20 << ui->grid_21 << ui->grid_22;

    ui->grid_00->setRow(0);
    ui->grid_00->setCol(0);
    ui->grid_01->setRow(0);
    ui->grid_01->setCol(1);
    ui->grid_02->setRow(0);
    ui->grid_02->setCol(2);
    ui->grid_10->setRow(1);
    ui->grid_10->setCol(0);
    ui->grid_11->setRow(1);
    ui->grid_11->setCol(1);
    ui->grid_12->setRow(1);
    ui->grid_12->setCol(2);
    ui->grid_20->setRow(2);
    ui->grid_20->setCol(0);
    ui->grid_21->setRow(2);
    ui->grid_21->setCol(1);
    ui->grid_22->setRow(2);
    ui->grid_22->setCol(2);

    for(int i = 0; i < list_grid.length(); ++i) {
        connect(list_grid.at(i), SIGNAL(clicked(int,int)), this, SLOT(labelClicked(int,int)));
    }

    connect(this->ui->actionAbout, SIGNAL(triggered()), this, SLOT(on_action_about_triggered()));
    connect(ui->button_giveUp, SIGNAL(clicked()), this, SLOT(giveUpPressed()));
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(this, SIGNAL(challenge(QString)), &client, SLOT(challenge(QString)));
    connect(this, SIGNAL(giveUp(QString)), &client, SLOT(giveUp(QString)));
    connect(this, SIGNAL(challengeAccepted(QString)), &client, SLOT(challengeAccepted(QString)));
    connect(this, SIGNAL(challengeRejected(QString)), &client, SLOT(challengeRejected(QString)));
    connect(this, SIGNAL(alreadyInGame(QString)), &client, SLOT(alreadyInGame(QString)));
    connect(this, SIGNAL(challengeOk(QString)), &client, SLOT(challengeOk(QString)));
    connect(this, SIGNAL(challengeNotOk(QString)), &client, SLOT(challengeNotOk(QString)));
    connect(this, SIGNAL(pressedGrid00(QString)), &client, SLOT(pressedGrid00(QString)));
    connect(this, SIGNAL(pressedGrid01(QString)), &client, SLOT(pressedGrid01(QString)));
    connect(this, SIGNAL(pressedGrid02(QString)), &client, SLOT(pressedGrid02(QString)));
    connect(this, SIGNAL(pressedGrid10(QString)), &client, SLOT(pressedGrid10(QString)));
    connect(this, SIGNAL(pressedGrid11(QString)), &client, SLOT(pressedGrid11(QString)));
    connect(this, SIGNAL(pressedGrid12(QString)), &client, SLOT(pressedGrid12(QString)));
    connect(this, SIGNAL(pressedGrid20(QString)), &client, SLOT(pressedGrid20(QString)));
    connect(this, SIGNAL(pressedGrid21(QString)), &client, SLOT(pressedGrid21(QString)));
    connect(this, SIGNAL(pressedGrid22(QString)), &client, SLOT(pressedGrid22(QString)));
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(&client, SIGNAL(newMessage(QString,QString)), this, SLOT(appendMessage(QString,QString)));
    connect(&client, SIGNAL(newChallenge(QString)), this, SLOT(displayChallenged(QString)));
    connect(&client, SIGNAL(gotChallengeAccepted(QString)), this, SLOT(displayChallengeAccepted(QString)));
    connect(&client, SIGNAL(gotChallengeRejected(QString)), this, SLOT(displayChallengeRejected(QString)));
    connect(&client, SIGNAL(gotAlreadyInGame(QString)), this, SLOT(displayAlreadyInGame(QString)));
    connect(&client, SIGNAL(gotChallengeOk(QString)), this, SLOT(displayChallengeOk(QString)));
    connect(&client, SIGNAL(gotChallengeNotOk(QString)), this, SLOT(displayChallengeNotOk(QString)));
    connect(&client, SIGNAL(newParticipant(QString)), this, SLOT(newParticipant(QString)));
    connect(&client, SIGNAL(participantLeft(QString)), this, SLOT(participantLeft(QString)));
    connect(&client, SIGNAL(newGiveUp(QString)), this, SLOT(displayOpponentGaveUp(QString)));
    connect(&client, SIGNAL(newPressedGrid00(QString)), this, SLOT(opponentPressedGrid00(QString)));
    connect(&client, SIGNAL(newPressedGrid01(QString)), this, SLOT(opponentPressedGrid01(QString)));
    connect(&client, SIGNAL(newPressedGrid02(QString)), this, SLOT(opponentPressedGrid02(QString)));
    connect(&client, SIGNAL(newPressedGrid10(QString)), this, SLOT(opponentPressedGrid10(QString)));
    connect(&client, SIGNAL(newPressedGrid11(QString)), this, SLOT(opponentPressedGrid11(QString)));
    connect(&client, SIGNAL(newPressedGrid12(QString)), this, SLOT(opponentPressedGrid12(QString)));
    connect(&client, SIGNAL(newPressedGrid20(QString)), this, SLOT(opponentPressedGrid20(QString)));
    connect(&client, SIGNAL(newPressedGrid21(QString)), this, SLOT(opponentPressedGrid21(QString)));
    connect(&client, SIGNAL(newPressedGrid22(QString)), this, SLOT(opponentPressedGrid22(QString)));
    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_action_challenged(QModelIndex)));

    myNickName = client.nickName();
    newParticipant(myNickName);
    tableFormat.setBorder(0);

    label_statusbar = new QLabel(this);
    ui->statusBar->addPermanentWidget(label_statusbar);

    reset();
}

MainWindow::~MainWindow()
{
    if(game) {
        delete game;
    }
    if(img_empty) {
        delete img_empty;
    }
    if(img_x) {
        delete img_x;
    }
    if(img_o) {
        delete img_o;
    }
    if(about) {
        delete about;
    }
    if(ui) {
        delete ui;
    }
}

void MainWindow::showMsgOk(QString title, QString msg)
{
    if(msgBox) {
        delete msgBox;
    }
    msgBox = new QMessageBox(this);

    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setText(QString(title));
    msgBox->setInformativeText(QString(msg));
    msgBox->exec();
}

int MainWindow::showMsgYesNo(QString title, QString msg)
{
    if(msgBox) {
        delete msgBox;
    }
    msgBox = new QMessageBox(this);

    msgBox->setText(title);
    msgBox->setInformativeText(msg);
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox->setDefaultButton(QMessageBox::Yes);
    int ret = msgBox->exec();
    return ret;
}

void MainWindow::clear_grid()
{
    for(int i = 0; i < list_grid.length(); ++i) {
        list_grid.at(i)->setText("");
        list_grid.at(i)->setPixmap(*img_empty);
    }
}

void MainWindow::reset()
{
    if(game) {
        delete game;
        game = 0;
    }
    gameInPreparation = false;
    opponent = "";
    ui->button_giveUp->setEnabled(false);
    label_statusbar->setText("Not playing.");
    clear_grid();
    for(int i = 0; i < list_grid.length(); ++i) {
        connect(list_grid.at(i), SIGNAL(clicked(int,int)), this, SLOT(labelClicked(int,int)));
    }
}

void MainWindow::startGame(QString opponent, bool playersMove)
{
    if(game) {
        delete game;
        game = 0;
    }
    game = new Game(opponent, playersMove);
    if(game->isValidGame()) {
        ui->button_giveUp->setEnabled(true);
        if(playersMove) {
            label_statusbar->setText("Your move");
        } else {
            label_statusbar->setText(QString("%1's move").arg(game->getOpponentName()));
        }
        clear_grid();
    } else {
        showMsgOk("Invalid game", "The game is not valid. Aborting...");
    }
}

bool MainWindow::isValidGridTile(int row, int col)
{
    if(game && game->isValidGame()) {
        return game->isEmptyTileInGrid(row, col);
    }
    return false;
}

void MainWindow::checkWinLose()
{
    if(game->playerWon()) {
        showMsgOk("You won!", QString("You bet your opponent '%1'!").arg(game->getOpponentName()));
        reset();
    } else if(game->opponentWon()) {
        showMsgOk("You lost!", QString("Your opponent '%1' bet you!").arg(game->getOpponentName()));
        reset();
    } else if(game->isADraw()) {
        showMsgOk("Draw!", QString("It's a draw between your opponent '%1' and you!").arg(game->getOpponentName()));
        reset();
    }
}

void MainWindow::labelClicked(int row, int col)
{
    if(game && game->isValidGame()) {
        if(game->isPlayersMove()) {
            switch(row)
            {
            case 0:
                switch(col)
                {
                case 0:
                    if(isValidGridTile(row, col)) {
                        emit pressedGrid00(QString("%1:%2").arg(game->getOpponentIpAddress()).arg(game->getOpponentPort()));
                        game->setInGrid(row, col, true);
                        game->setPlayersMove(false);
                        ui->grid_00->setPixmap(*img_o);
                        label_statusbar->setText(QString("%1's move").arg(game->getOpponentName()));
                        checkWinLose();
                    }
                    break;
                case 1:
                    if(isValidGridTile(row, col)) {
                        emit pressedGrid01(QString("%1:%2").arg(game->getOpponentIpAddress()).arg(game->getOpponentPort()));
                        game->setInGrid(row, col, true);
                        game->setPlayersMove(false);
                        ui->grid_01->setPixmap(*img_o);
                        label_statusbar->setText(QString("%1's move").arg(game->getOpponentName()));
                        checkWinLose();
                    }
                    break;
                case 2:
                    if(isValidGridTile(row, col)) {
                        emit pressedGrid02(QString("%1:%2").arg(game->getOpponentIpAddress()).arg(game->getOpponentPort()));
                        game->setInGrid(row, col, true);
                        game->setPlayersMove(false);
                        ui->grid_02->setPixmap(*img_o);
                        label_statusbar->setText(QString("%1's move").arg(game->getOpponentName()));
                        checkWinLose();
                    }
                    break;
                default:
                    break;
                }
                break;
            case 1:
                switch(col)
                {
                case 0:
                    if(isValidGridTile(row, col)) {
                        emit pressedGrid10(QString("%1:%2").arg(game->getOpponentIpAddress()).arg(game->getOpponentPort()));
                        game->setInGrid(row, col, true);
                        game->setPlayersMove(false);
                        ui->grid_10->setPixmap(*img_o);
                        label_statusbar->setText(QString("%1's move").arg(game->getOpponentName()));
                        checkWinLose();
                    }
                    break;
                case 1:
                    if(isValidGridTile(row, col)) {
                        emit pressedGrid11(QString("%1:%2").arg(game->getOpponentIpAddress()).arg(game->getOpponentPort()));
                        game->setInGrid(row, col, true);
                        game->setPlayersMove(false);
                        ui->grid_11->setPixmap(*img_o);
                        label_statusbar->setText(QString("%1's move").arg(game->getOpponentName()));
                        checkWinLose();
                    }
                    break;
                case 2:
                    if(isValidGridTile(row, col)) {
                        emit pressedGrid12(QString("%1:%2").arg(game->getOpponentIpAddress()).arg(game->getOpponentPort()));
                        game->setInGrid(row, col, true);
                        game->setPlayersMove(false);
                        ui->grid_12->setPixmap(*img_o);
                        label_statusbar->setText(QString("%1's move").arg(game->getOpponentName()));
                        checkWinLose();
                    }
                    break;
                default:
                    break;
                }
                break;
            case 2:
                switch(col)
                {
                case 0:
                    if(isValidGridTile(row, col)) {
                        emit pressedGrid20(QString("%1:%2").arg(game->getOpponentIpAddress()).arg(game->getOpponentPort()));
                        game->setInGrid(row, col, true);
                        game->setPlayersMove(false);
                        ui->grid_20->setPixmap(*img_o);
                        label_statusbar->setText(QString("%1's move").arg(game->getOpponentName()));
                        checkWinLose();
                    }
                    break;
                case 1:
                    if(isValidGridTile(row, col)) {
                        emit pressedGrid21(QString("%1:%2").arg(game->getOpponentIpAddress()).arg(game->getOpponentPort()));
                        game->setInGrid(row, col, true);
                        game->setPlayersMove(false);
                        ui->grid_21->setPixmap(*img_o);
                        label_statusbar->setText(QString("%1's move").arg(game->getOpponentName()));
                        checkWinLose();
                    }
                    break;
                case 2:
                    if(isValidGridTile(row, col)) {
                        emit pressedGrid22(QString("%1:%2").arg(game->getOpponentIpAddress()).arg(game->getOpponentPort()));
                        game->setInGrid(row, col, true);
                        game->setPlayersMove(false);
                        ui->grid_22->setPixmap(*img_o);
                        label_statusbar->setText(QString("%1's move").arg(game->getOpponentName()));
                        checkWinLose();
                    }
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty()) {
        return;
    }

    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);

    QString _from;
    if(from.contains("@")) {
        QStringList l = from.split("@");
        if(l.length() == 2) {
            _from = l.at(0);
        } else {
            _from = from;
        }
    } else {
        _from = from;
    }

    table->cellAt(0, 0).firstCursorPosition().insertText("<" + _from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void MainWindow::displayChallenged(const QString &from)
{
    if (from.isEmpty()) {
        return;
    }

    if(from.contains("@")) {
        QStringList l = from.split("@");
        if(l.length() == 2) {
            if(game) {
                emit alreadyInGame(l.at(1));
            } else if(gameInPreparation) {
                emit alreadyInGame(l.at(1));
            } else {
                gameInPreparation = true;
                int ret = showMsgYesNo("You got challenged", QString("Player '%1' challenged you. Accept the challenge?").arg(l.at(0)));
                switch (ret) {
                case QMessageBox::Yes:
                    emit challengeAccepted(l.at(1));
                    break;
                default:
                case QMessageBox::No:
                    gameInPreparation = false;
                    emit challengeRejected(l.at(1));
                    break;
                }
            }
        }
    }
}

void MainWindow::displayChallengeAccepted(const QString &from)
{
    if (from.isEmpty()) {
        return;
    }

    if(from.contains("@")) {
        QStringList l = from.split("@");
        if(l.length() == 2) {
            if(game) {
                emit alreadyInGame(l.at(1));
            } else {
                opponent = QString(from);
                emit challengeOk(l.at(1));
                startGame(from, challengingPlayerHasFirstMove);
                showMsgOk("Challenge accepted", QString("Player '%1' accepted the challenge. The game starts now!").arg(l.at(0)));
            }
        } else {
            emit challengeNotOk(l.at(1));
        }
    }
}

void MainWindow::displayChallengeRejected(const QString &from)
{
    if (from.isEmpty()) {
        return;
    }

    reset();
    if(from.contains("@")) {
        QStringList l = from.split("@");
        if(l.length() == 2) {
            if(!game) {
                showMsgOk("Challenge rejected", QString("Player '%1' rejected the challenge.").arg(l.at(0)));
                reset();
            }
        }
    }
}

void MainWindow::displayAlreadyInGame(const QString &from)
{
    if (from.isEmpty()) {
        return;
    }

    reset();
    if(from.contains("@")) {
        QStringList l = from.split("@");
        if(l.length() == 2) {
            if(!game) {
                showMsgOk("Player already in game", QString("Player '%1' is already playing a game.").arg(l.at(0)));
            }
        }
    }
}

void MainWindow::displayChallengeOk(const QString &from)
{
    if (from.isEmpty()) {
        return;
    }

    if(from.contains("@")) {
        QStringList l = from.split("@");
        if(l.length() == 2) {
            if(!game) {
                opponent = QString(from);
                startGame(from, !challengingPlayerHasFirstMove);
            } else {
                reset();
            }
        }
    }
}

void MainWindow::displayChallengeNotOk(const QString &from)
{
    if (from.isEmpty()) {
        return;
    }

    if(from.contains("@")) {
        QStringList l = from.split("@");
        if(l.length() == 2) {
            if(!game) {
                reset();
                showMsgOk("Not available", QString("Player '%1' accepted the challenge, but is not available any more.").arg(l.at(0)));
            }
        }
    }
}

void MainWindow::displayOpponentGaveUp(const QString &from)
{
    if (from.isEmpty()) {
        return;
    }

    if(game && game->isValidGame()) {
        reset();
        if(from.contains("@")) {
            QStringList l = from.split("@");
            if(l.length() == 2) {
                reset();
                showMsgOk("You won!", QString("Player '%1' gave up and lost the game.").arg(l.at(0)));
            }
        }
    }
}

void MainWindow::opponentPressedGrid00(const QString &from) {
    if(game && game->isOpponent(from) && game->isValidGame() && !game->isPlayersMove()) {
        game->setInGrid(0, 0, false);
        game->setPlayersMove(true);
        ui->grid_00->setPixmap(*img_x);
        label_statusbar->setText("Your move");
        checkWinLose();
    }
}

void MainWindow::opponentPressedGrid01(const QString &from) {
    if(game && game->isOpponent(from) && game->isValidGame() && !game->isPlayersMove()) {
        game->setInGrid(0, 1, false);
        game->setPlayersMove(true);
        ui->grid_01->setPixmap(*img_x);
        label_statusbar->setText("Your move");
        checkWinLose();
    }
}

void MainWindow::opponentPressedGrid02(const QString &from) {
    if(game && game->isOpponent(from) && game->isValidGame() && !game->isPlayersMove()) {
        game->setInGrid(0, 2, false);
        game->setPlayersMove(true);
        ui->grid_02->setPixmap(*img_x);
        label_statusbar->setText("Your move");
        checkWinLose();
    }
}

void MainWindow::opponentPressedGrid10(const QString &from) {
    if(game && game->isOpponent(from) && game->isValidGame() && !game->isPlayersMove()) {
        game->setInGrid(1, 0, false);
        game->setPlayersMove(true);
        ui->grid_10->setPixmap(*img_x);
        label_statusbar->setText("Your move");
        checkWinLose();
    }
}

void MainWindow::opponentPressedGrid11(const QString &from) {
    if(game && game->isOpponent(from) && game->isValidGame() && !game->isPlayersMove()) {
        game->setInGrid(1, 1, false);
        game->setPlayersMove(true);
        ui->grid_11->setPixmap(*img_x);
        label_statusbar->setText("Your move");
        checkWinLose();
    }
}

void MainWindow::opponentPressedGrid12(const QString &from) {
    if(game && game->isOpponent(from) && game->isValidGame() && !game->isPlayersMove()) {
        game->setInGrid(1, 2, false);
        game->setPlayersMove(true);
        ui->grid_12->setPixmap(*img_x);
        label_statusbar->setText("Your move");
        checkWinLose();
    }
}

void MainWindow::opponentPressedGrid20(const QString &from) {
    if(game && game->isOpponent(from) && game->isValidGame() && !game->isPlayersMove()) {
        game->setInGrid(2, 0, false);
        game->setPlayersMove(true);
        ui->grid_20->setPixmap(*img_x);
        label_statusbar->setText("Your move");
        checkWinLose();
    }
}

void MainWindow::opponentPressedGrid21(const QString &from) {
    if(game && game->isOpponent(from) && game->isValidGame() && !game->isPlayersMove()) {
        game->setInGrid(2, 1, false);
        game->setPlayersMove(true);
        ui->grid_21->setPixmap(*img_x);
        label_statusbar->setText("Your move");
        checkWinLose();
    }
}

void MainWindow::opponentPressedGrid22(const QString &from) {
    if(game && game->isOpponent(from) && game->isValidGame() && !game->isPlayersMove()) {
        game->setInGrid(2, 2, false);
        game->setPlayersMove(true);
        ui->grid_22->setPixmap(*img_x);
        label_statusbar->setText("Your move");
        checkWinLose();
    }
}

void MainWindow::on_action_about_triggered()
{
    about->setVisible(true);
}

void MainWindow::on_action_challenged(const QModelIndex &index)
{
    if(!game && !gameInPreparation) {
        const QAbstractItemModel* model = index.model();
        QVariant qv = model->data(index);
        QString fullName(qv.toString());
        if(fullName.contains("@")) {
            QStringList l = fullName.split("@");
            if(l.length() == 2) {
                int ret = showMsgYesNo(QString("Challenge player '%1'").arg(l.at(0)), QString("Do you want to challenge player %1?").arg(l.at(0)));
                switch (ret) {
                case QMessageBox::Yes:
                    if(!gameInPreparation) {
                        gameInPreparation = true;
                        emit challenge(l.at(1));
                    }
                    break;
                default:
                case QMessageBox::No:
                    break;
                }
            }
        } else {
            showMsgOk("Error", "You can't challenge yourself...");
        }
    } else {
        showMsgOk("Finish your game!", "You can't challenge anyone, you're in the middle of a game right now...");
    }
}

void MainWindow::returnPressed()
{
    QString text = ui->lineEdit->text();
    if (text.isEmpty()) {
        return;
    }

    client.sendMessage(text);
    appendMessage(myNickName, text);

    ui->lineEdit->clear();
}

void MainWindow::giveUpPressed()
{
    if(game && game->isValidGame()) {
        if(opponent.contains("@")) {
            QStringList l = opponent.split("@");
            if(l.length() == 2) {
                int ret = showMsgYesNo("Give up and lose", "Do you want to give up and lose the game?");
                switch (ret) {
                case QMessageBox::Yes:
                {
                    if(game && game->isValidGame()) {
                        emit giveUp(l.at(1));
                        reset();
                        showMsgOk("You lost!", "You gave up and lost the game.");
                    }
                }
                    break;
                default:
                case QMessageBox::No:
                    break;
                }
            } else {
                reset();
            }
        } else {
            reset();
        }
    }
}

void MainWindow::newParticipant(const QString &nick)
{
    if (nick.isEmpty()) {
        return;
    }

    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);

    QString _nick;
    if(nick.contains("@")) {
        QStringList l = nick.split("@");
        if(l.length() == 2) {
            _nick = l.at(0);
        } else {
            _nick = nick;
        }
    } else {
        _nick = nick;
    }

    ui->textEdit->append(tr("* %1 has joined").arg(_nick));
    ui->textEdit->setTextColor(color);
    ui->listWidget->addItem(nick);
}

void MainWindow::participantLeft(const QString &nick)
{
    if (nick.isEmpty()) {
        return;
    }

    QList<QListWidgetItem *> items = ui->listWidget->findItems(nick, Qt::MatchExactly);
    if (items.isEmpty()) {
        return;
    }

    QString _nick;
    if(nick.contains("@")) {
        QStringList l = nick.split("@");
        if(l.length() == 2) {
            _nick = l.at(0);
        } else {
            _nick = nick;
        }
    } else {
        _nick = nick;
    }

    if(game && game->isOpponent(nick)) {
        showMsgOk("You won!", QString("Player '%1' left and lost the game.").arg(_nick));
        reset();
    }

    delete items.at(0);
    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has left").arg(_nick));
    ui->textEdit->setTextColor(color);
}
