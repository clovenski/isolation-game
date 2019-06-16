#include "../include/State.h"
#include <climits>
#include <cmath>
#include <algorithm>
using namespace std;

const int State::POS_OFFSETS[8][2][7] = {
    { // axis 0: up
        {-1, -2, -3, -4, -5, -6, -7},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // axis 1: up-right
        {-1, -2, -3, -4, -5, -6, -7},
        {1, 2, 3, 4, 5, 6, 7}
    },
    { // axis 2: right
        {0, 0, 0, 0, 0, 0, 0},
        {1, 2, 3, 4, 5, 6, 7}
    },
    { // axis 3: down-right
        {1, 2, 3, 4, 5, 6, 7},
        {1, 2, 3, 4, 5, 6, 7}
    },
    { // axis 4: down
        {1, 2, 3, 4, 5, 6, 7},
        {0, 0, 0, 0, 0, 0, 0}
    },
    { // axis 5: down-left
        {1, 2, 3, 4, 5, 6, 7},
        {-1, -2, -3, -4, -5, -6, -7}
    },
    { // axis 6: left
        {0, 0, 0, 0, 0, 0, 0},
        {-1, -2, -3, -4, -5, -6, -7}
    },
    { // axis 7: up-left
        {-1, -2, -3, -4, -5, -6, -7},
        {-1, -2, -3, -4, -5, -6, -7}
    }
};

const string State::NO_WINNER = "None";
const string State::WINNER_X = "X";
const string State::WINNER_O = "O";

State::State(bool xTop) {
    int i, j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            board[i][j] = '-';
        }
    }

    utility = 0;

    this->xTop = xTop;

    board[0][0] = xTop ? 'X' : 'O';
    board[7][7] = xTop ? 'O' : 'X';
    if (xTop) {
        xPos = {0, 0};
        oPos = {7, 7};
    } else {
        xPos = {7, 7};
        oPos = {0, 0};
    }
    xMoves = oMoves = 20;
    xLocalMoves = oLocalMoves = 3;

    winner = &NO_WINNER;
    xTurn = xTop;
}

void State::setWhoFirst(bool xFirst) {
    xTop = xFirst;
}

bool State::validIndex(int index) {
    if (index >= 0 && index < 8) {
        return true;
    } else {
        return false;
    }
}

void State::computeUtility() {
    int i, j, targetRow, targetCol;
    xLocalMoves = oLocalMoves = 0;
    bool checkAxis[8] = {false};

    // count local moves for X
    for (i = 0; i < 8; i++) {
        targetRow = xPos.row + State::POS_OFFSETS[i][0][0];
        if (!State::validIndex(targetRow)) {
            continue;
        }
        targetCol = xPos.col + State::POS_OFFSETS[i][1][0];
        if (!State::validIndex(targetCol)) {
            continue;
        }

        if (board[targetRow][targetCol] == '-') {
            xLocalMoves++;
            checkAxis[i] = true;
        }
    }

    xMoves = xLocalMoves;
    // count rest of moves for X
    for (i = 0; i < 8; i++) {
        if (checkAxis[i]) {
            for (j = 1; j < 8; j++) {
                targetRow = xPos.row + State::POS_OFFSETS[i][0][j];
                if (!State::validIndex(targetRow)) {
                    break;
                }
                targetCol = xPos.col + State::POS_OFFSETS[i][1][j];
                if (!State::validIndex(targetCol)) {
                    break;
                }

                if (board[targetRow][targetCol] == '-') {
                    xMoves++;
                } else {
                    break;
                }
            }
        }
    }

    for (i = 0; i < 8; i++) {
        checkAxis[i] = false;
    }

    // count local moves for O
    for (i = 0; i < 8; i++) {
        targetRow = oPos.row + State::POS_OFFSETS[i][0][0];
        if (!State::validIndex(targetRow)) {
            continue;
        }
        targetCol = oPos.col + State::POS_OFFSETS[i][1][0];
        if (!State::validIndex(targetCol)) {
            continue;
        }

        if (board[targetRow][targetCol] == '-') {
            oLocalMoves++;
            checkAxis[i] = true;
        }
    }

    oMoves = oLocalMoves;
    // count rest of moves for O
    for (i = 0; i < 8; i++) {
        if (checkAxis[i]) {
            for (j = 1; j < 8; j++) {
                targetRow = oPos.row + State::POS_OFFSETS[i][0][j];
                if (!State::validIndex(targetRow)) {
                    break;
                }
                targetCol = oPos.col + State::POS_OFFSETS[i][1][j];
                if (!State::validIndex(targetCol)) {
                    break;
                }

                if (board[targetRow][targetCol] == '-') {
                    oMoves++;
                } else {
                    break;
                }
            }
        }
    }

    if (xMoves != 0 && oMoves != 0) {
        utility = xMoves + xLocalMoves - oMoves - oLocalMoves;
        winner = &NO_WINNER;
    } else if (xMoves == 0 && oMoves == 0) {
        utility = xTurn ? INT_MIN : INT_MAX;
        winner = xTurn ? &WINNER_O : &WINNER_X;
    } else if (xMoves == 0) {
        utility = INT_MIN;
        winner = &WINNER_O;
    } else {
        utility = INT_MAX;
        winner = &WINNER_X;
    }
}

State::Sorter::Sorter(bool forX, Position xPos, int xMoves, Position oPos, int oMoves) {
    this->forX = forX;
    this->xPos = xPos;
    this->xMoves = xMoves;
    this->oPos = oPos;
    this->oMoves = oMoves;
}

int State::Sorter::dist(Position p1, Position p2) {
    return abs(p1.row - p2.row) + abs(p1.col - p2.col);
}

int State::Sorter::distFromCent(Position pos) {
    return abs(pos.row - 3) + abs(pos.col - 3);
}

bool State::Sorter::operator() (Position p1, Position p2) {
    int dist1, dist2;

    if ((forX && oMoves < 16) || (!forX && xMoves < 16)) {
        if (forX) {
            dist1 = this->dist(p1, oPos);
            dist2 = this->dist(p2, oPos);
        } else {
            dist1 = this->dist(p1, xPos);
            dist2 = this->dist(p2, xPos);
        }

    } else {
        dist1 = this->distFromCent(p1);
        dist2 = this->distFromCent(p2);
    }

    return dist1 < dist2;
}

vector<Position> State::getSuccessors(bool ofX) {
    static Sorter sorter(true, {0,0}, 0, {0,0}, 0);
    vector<Position> successors;
    int axis, i, targetRow, targetCol;
    Position successor;

    Position pos = ofX ? xPos : oPos;

    for (axis = 0; axis < 8; axis++) {
        for (i = 0; i < 8; i++) {
            targetRow = pos.row + State::POS_OFFSETS[axis][0][i];
            if (!State::validIndex(targetRow)) {
                break;
            }
            targetCol = pos.col + State::POS_OFFSETS[axis][1][i];
            if (!State::validIndex(targetCol)) {
                break;
            }

            if (board[targetRow][targetCol] == '-') {
                successor = {targetRow, targetCol};
                successors.push_back(successor);
            } else {
                break;
            }
        }
    }

    // sort successors
    sorter.forX = ofX;
    sorter.xPos.row = xPos.row;
    sorter.xPos.col = xPos.col;
    sorter.xMoves = xMoves;
    sorter.oPos.row = oPos.row;
    sorter.oPos.col = oPos.col;
    sorter.oMoves = oMoves;
    sort(successors.begin(), successors.end(), sorter);

    return successors;
}

Position State::getXPos() {
    return xPos;
}

Position State::getOPos() {
    return oPos;
}

void State::move(bool movingX, Position pos) {
    board[pos.row][pos.col] = movingX ? 'X' : 'O';

    if (movingX) {
        board[xPos.row][xPos.col] = '#';
        xPos = pos;
    } else {
        board[oPos.row][oPos.col] = '#';
        oPos = pos;
    }

    xTurn = !movingX;
    State::computeUtility();
}

void State::revert(bool movingX, Position pos) {
    board[pos.row][pos.col] = movingX ? 'X' : 'O';

    if (movingX) {
        board[xPos.row][xPos.col] = '-';
        xPos = pos;
    } else {
        board[oPos.row][oPos.col] = '-';
        oPos = pos;
    }

    xTurn = movingX;
    State::computeUtility();
}

bool State::isTerminal() {
    return xTurn ? xLocalMoves == 0 : oLocalMoves == 0;
}

int State::getUtility() {
    return utility;
}

string State::getWinner() {
    return *winner;
}

void State::reset() {
    int i, j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            board[i][j] = '-';
        }
    }

    utility = 0;

    board[0][0] = xTop ? 'X' : 'O';
    board[7][7] = xTop ? 'O' : 'X';
    if (xTop) {
        xPos = {0, 0};
        oPos = {7, 7};
    } else {
        xPos = {7, 7};
        oPos = {0, 0};
    }
    xMoves = oMoves = 20;
    xLocalMoves = oLocalMoves = 3;

    winner = &NO_WINNER;
    xTurn = xTop;
}

string State::toString() {
    string result = "";
    int i, j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            result += board[i][j];
        }
    }

    return result;
}
