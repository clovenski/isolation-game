#include "../include/Engine.h"
using namespace std;

Position Engine::getRandCompMove() {
    srand(time(NULL));
    vector<Position> moves = state.getSuccessors(true);
    return moves.at(rand() % moves.size());
}

Engine::Engine(bool xFirst = true, double timeLimit = 2.0) : TIME_LIMIT(timeLimit), state(xFirst) {
    this->xFirst = xFirst;
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
