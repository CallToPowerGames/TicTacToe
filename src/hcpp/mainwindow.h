/**
 * TicTacToe
 *
 * Copyright 2014 Denis Meyer, CallToPower Software
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPixmap>
#include <QLabel>
#include <QMessageBox>
#include "clickablelabel.h"
#include "textwindow.h"
#include "client.h"
#include "game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<ClickableLabel* > list_grid;
    TextWindow *about;
    Client client;
    Game *game;
    QMessageBox *msgBox;
    QString myNickName;
    QTextTableFormat tableFormat;
    QPixmap *img_empty;
    QPixmap *img_x;
    QPixmap *img_o;
    QLabel *label_statusbar;
    QString opponent;
    bool challengingPlayerHasFirstMove;
    bool gameInPreparation;
    void showMsgOk(QString title, QString msg);
    int showMsgYesNo(QString title, QString msg);
    void clear_grid();
    void reset();
    void startGame(QString opponent, bool playersMove);
    bool isValidGridTile(int row, int col);
    void checkWinLose();

signals:
    void giveUp(const QString to);
    void challenge(const QString to);
    void challengeAccepted(const QString to);
    void challengeRejected(const QString to);
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

public slots:
    void labelClicked(int row, int col);
    void appendMessage(const QString &from, const QString &message);
    void displayChallenged(const QString &from);
    void displayChallengeAccepted(const QString &from);
    void displayChallengeRejected(const QString &from);
    void displayAlreadyInGame(const QString &from);
    void displayChallengeOk(const QString &from);
    void displayChallengeNotOk(const QString &from);
    void displayOpponentGaveUp(const QString &from);
    void opponentPressedGrid00(const QString &from);
    void opponentPressedGrid01(const QString &from);
    void opponentPressedGrid02(const QString &from);
    void opponentPressedGrid10(const QString &from);
    void opponentPressedGrid11(const QString &from);
    void opponentPressedGrid12(const QString &from);
    void opponentPressedGrid20(const QString &from);
    void opponentPressedGrid21(const QString &from);
    void opponentPressedGrid22(const QString &from);

private slots:
    void on_action_about_triggered();
    void on_action_challenged(const QModelIndex &index);
    void returnPressed();
    void giveUpPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
};

#endif // MAINWINDOW_H
