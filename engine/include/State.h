#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Position {
    int row;
    int col;
    inline bool operator==(const Position& rhs) {
        return row == rhs.row && col == rhs.col;
    }
};

class State {
    private:
        char board[8][8];
        int utility;
        bool xTop;
        Position xPos;
        Position oPos;
        int xMoves;
        int xLocalMoves;
        int oMoves;
        int oLocalMoves;
        const std::string *winner;
        bool xTurn;
        static const std::string NO_WINNER;
        static const std::string WINNER_X;
        static const std::string WINNER_O;
        static const int POS_OFFSETS[8][2][7];
        void computeUtility();
        bool validIndex(int);
        struct Sorter {
            bool forX;
            Position xPos;
            int xMoves;
            Position oPos;
            int oMoves;
            bool operator() (Position, Position);
            int dist(Position, Position);
            int distFromCent(Position);
            Sorter(bool, Position, int, Position, int);
        };
    public:
        State(bool);
        void setWhoFirst(bool);
        std::vector<Position> getSuccessors(bool);
        Position getXPos();
        Position getOPos();
        void move(bool, Position);
        void revert(bool, Position);
        bool isTerminal();
        int getUtility();
        std::string getWinner();
        void reset();
        std::string toString(int);
        std::string toString();
};