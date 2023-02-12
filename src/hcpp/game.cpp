/**
 * TicTacToe
 *
 * Copyright 2014 Denis Meyer, CallToPower Software
 */
#include "game.h"
#include <QStringList>

Game::Game(QString _opponent, bool _playersMove = true)
    : opponentParsed(false), opponent(_opponent), opponentName(""), opponentIpAddress(""), opponentPort(0), playersMove(_playersMove)
{
    parseOpponent();
}

Game::Game(QString _opponentName, QString _opponentIpAddress, int _opponentPort, bool _playersMove = true)
    : opponentParsed(true), opponent(""), opponentName(_opponentName), opponentIpAddress(_opponentIpAddress), opponentPort(_opponentPort), playersMove(_playersMove)
{
    createOpponent();
}

Game::~Game()
{
}

bool Game::isValidGame()
{
    return opponentParsed;
}

bool Game::isPlayersMove()
{
    return playersMove;
}

QString Game::getOpponent()
{
    return opponent;
}

QString Game::getOpponentName()
{
    return opponentName;
}

QString Game::getOpponentIpAddress()
{
    return opponentIpAddress;
}

int Game::getOpponentPort()
{
    return opponentPort;
}

bool Game::isOpponent(QString _opponent)
{
    return opponent.compare(_opponent) == 0;
}

void Game::setPlayersMove(bool _playersMove)
{
    playersMove = _playersMove;
}

void Game::setOpponent(QString _opponent)
{
    if(!_opponent.isEmpty()) {
        opponent = _opponent;
        parseOpponent();
    }
}

void Game::setOpponentName(QString _opponentName)
{
    if(!_opponentName.isEmpty()) {
        opponentName = _opponentName;
        createOpponent();
    }
}

void Game::setOpponentIpAddress(QString _opponentIpAddress)
{
    if(!_opponentIpAddress.isEmpty()) {
        opponentIpAddress = _opponentIpAddress;
        createOpponent();
    }
}

void Game::setOpponentPort(int _opponentPort)
{
    if(_opponentPort >= 0) {
        opponentPort = _opponentPort;
        createOpponent();
    }
}

void Game::clearGrid()
{
    for(int i = 0; i <= 2; ++i) {
        for(int j = 0; j <= 2; ++j) {
            grid[i][j] = -1;
        }
    }
}

void Game::setInGrid(int row, int col, bool playersTile) {
    if((row >= 0) && (row <= 2) && (col >= 0) && (col <= 2)) {
        grid[row][col] = playersTile ? 1 : 0;
    }
}

bool Game::isEmptyTileInGrid(int row, int col) {
    if((row >= 0) && (row <= 2) && (col >= 0) && (col <= 2)) {
        return (grid[row][col] == -1) ? true : false;
    }
    return false;
}

bool Game::isPlayersTileInGrid(int row, int col) {
    if((row >= 0) && (row <= 2) && (col >= 0) && (col <= 2)) {
        return (grid[row][col] == 1) ? true : false;
    }
    return false;
}

bool Game::isOpponentsTileInGrid(int row, int col) {
    if((row >= 0) && (row <= 2) && (col >= 0) && (col <= 2)) {
        return (grid[row][col] == 0) ? true : false;
    }
    return false;
}

bool Game::isADraw() {
    bool fieldIsFull = true;
    for(int i = 0; i <= 2; ++i) {
        for(int j = 0; j <= 2; ++j) {
            if(grid[i][j] == -1) {
                fieldIsFull = false;
                break;
            }
        }
    }
    return !playerWon() && !opponentWon() && fieldIsFull;
}

bool Game::playerWon() {
    bool row0 = grid[0][0] == 1 && grid[0][1] == 1 && grid[0][2] == 1;
    bool row1 = grid[1][0] == 1 && grid[1][1] == 1 && grid[1][2] == 1;
    bool row2 = grid[2][0] == 1 && grid[2][1] == 1 && grid[2][2] == 1;
    bool col0 = grid[0][0] == 1 && grid[1][0] == 1 && grid[2][0] == 1;
    bool col1 = grid[0][1] == 1 && grid[1][1] == 1 && grid[2][1] == 1;
    bool col2 = grid[0][2] == 1 && grid[1][2] == 1 && grid[2][2] == 1;
    bool diag0 = grid[0][0] == 1 && grid[1][1] == 1 && grid[2][2] == 1;
    bool diag1 = grid[0][2] == 1 && grid[1][1] == 1 && grid[2][0] == 1;

    return row0 || row1 || row2 || col0 || col1 || col2 || diag0 || diag1;
}

bool Game::opponentWon() {
    bool row0 = grid[0][0] == 0 && grid[0][1] == 0 && grid[0][2] == 0;
    bool row1 = grid[1][0] == 0 && grid[1][1] == 0 && grid[1][2] == 0;
    bool row2 = grid[2][0] == 0 && grid[2][1] == 0 && grid[2][2] == 0;
    bool col0 = grid[0][0] == 0 && grid[1][0] == 0 && grid[2][0] == 0;
    bool col1 = grid[0][1] == 0 && grid[1][1] == 0 && grid[2][1] == 0;
    bool col2 = grid[0][2] == 0 && grid[1][2] == 0 && grid[2][2] == 0;
    bool diag0 = grid[0][0] == 0 && grid[1][1] == 0 && grid[2][2] == 0;
    bool diag1 = grid[0][2] == 0 && grid[1][1] == 0 && grid[2][0] == 0;

    return row0 || row1 || row2 || col0 || col1 || col2 || diag0 || diag1;
}

void Game::parseOpponent()
{
    opponentParsed = false;
    if(opponent.contains("@")) {
        QStringList l = opponent.split("@");
        if(l.length() == 2) {
            opponentName = l.at(0);
            if(l.at(1).contains(":")) {
                QStringList l2 = l.at(1).split(":");
                if(l2.length() == 2) {
                    opponentIpAddress = l2.at(0);
                    opponentPort= l2.at(1).toInt();
                    opponentParsed = true;
                }
            }
        }
    }
    clearGrid();
}

void Game::createOpponent()
{
    if(!opponentName.isEmpty() && !opponentIpAddress.isEmpty() && (opponentPort >= 0)) {
        opponent = QString("%1@%2:%3").arg(opponentName).arg(opponentIpAddress).arg(opponentPort);
        opponentParsed = true;
    } else {
        opponentParsed = false;
    }
    clearGrid();
}
