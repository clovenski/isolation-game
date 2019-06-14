#pragma once

#include "Minimax.h"
#include "State.h"

class Engine {
    private:
        State state;
        bool xFirst;
        const std::chrono::duration<double> TIME_LIMIT;
        int startDepth;
        double earlyStopLimit;
        Position getRandCompMove();
    public:
        Engine(bool, double);
        std::vector<Position> getChoices();
        Position getCompMove();
        void movePlayer(Position);
        bool terminalState();
        std::string getWinner();
        void reset();
};
