#include "../include/Engine.h"
using namespace std;

Position Engine::getRandCompMove() {
    srand(time(NULL));
    vector<Position> moves = state.getSuccessors(true);
    return moves.at(rand() % moves.size());
}

void Engine::processTimeLimit(double timeLimit) {
    if (timeLimit >= 1.0) {
        TIME_LIMIT = chrono::duration<double>(timeLimit);
    } else {
        TIME_LIMIT = chrono::duration<double>(2.0);
    }
    if (TIME_LIMIT.count() >= 20.0) {
        startDepth = 8;
        earlyStopLimit = 1.8;
    } else if (TIME_LIMIT.count() >= 7.0) {
        startDepth = 8;
        earlyStopLimit = 1.0;
    } else if (TIME_LIMIT.count() >= 3.0) {
        startDepth = 7;
        earlyStopLimit = 0.5;
    } else {
        startDepth = 6;
        earlyStopLimit = 0.5;
    }
    Minimax::earlyStopLimit = earlyStopLimit;
}

Engine::Engine(bool xFirst, double timeLimit) : state(xFirst) {
    this->xFirst = xFirst;
    processTimeLimit(timeLimit);
}

void Engine::setWhoFirst(bool xFirst) {
    state.setWhoFirst(xFirst);
    this->xFirst = xFirst;
}

void Engine::setTimeLimit(double timeLimit) {
    processTimeLimit(timeLimit);
}

vector<Position> Engine::getChoices() {
    return state.getSuccessors(false);
}

Position Engine::getCompMove() {
    if (!state.isTerminal()) {
        Position compMove, bestCompMove;
        int i, bestDepth, bestUtility;

        Minimax::timeRemaining = chrono::duration<double>(TIME_LIMIT);
        Minimax::resetTransTable();
        compMove = Minimax::search(true, state, startDepth);
        if (compMove.row == -1 || compMove.col == -1) {
            compMove = getRandCompMove();
        } else {
            bestDepth = startDepth;
            bestUtility = Minimax::getMaxUtility();
            for (i = startDepth + 1; i <= 40; i++) {
                bestCompMove = Minimax::search(true, state, i);
                if (bestCompMove.row != -1 && bestCompMove.col != -1) {
                    bestDepth = i;
                    compMove = bestCompMove;
                    bestUtility = Minimax::getMaxUtility();
                } else {
                    break;
                }
            }
        }
        return compMove;
    } else {
        return {-1,-1};
    }
}

void Engine::movePlayer(Position pos) {
    state.move(false, pos);
}

bool Engine::terminalState() {
    return state.isTerminal();
}

string Engine::getWinner() {
    return state.getWinner();
}

void Engine::reset() {
    state.reset();
}
