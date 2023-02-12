/**
 * TicTacToe
 *
 * Copyright 2014 Denis Meyer, CallToPower Software
 */
#ifndef GAME_H
#define GAME_H

#include <QString>

class Game
{
public:
    Game(QString _opponent, bool _playersMove);
    Game(QString _opponentName, QString _opponentIpAddress, int _opponentPort, bool _playersMove);
    ~Game();

    bool isValidGame();
    bool isPlayersMove();

    QString getOpponent();
    QString getOpponentName();
    QString getOpponentIpAddress();
    int getOpponentPort();
    bool isOpponent(QString _opponent);

    void setPlayersMove(bool _playersMove);
    void setOpponent(QString _opponent);
    void setOpponentName(QString _opponentName);
    void setOpponentIpAddress(QString _opponentIpAddress);
    void setOpponentPort(int _opponentPort);

    void clearGrid();
    void setInGrid(int row, int col, bool playersTile);
    bool isEmptyTileInGrid(int row, int col);
    bool isPlayersTileInGrid(int row, int col);
    bool isOpponentsTileInGrid(int row, int col);

    bool isADraw();
    bool playerWon();
    bool opponentWon();

private:
    bool opponentParsed;
    QString opponent;
    QString opponentName;
    QString opponentIpAddress;
    int opponentPort;
    bool playersMove;
    int grid[3][3];

    void parseOpponent();
    void createOpponent();
};

#endif // GAME_H
