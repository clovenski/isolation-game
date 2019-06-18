#include "../include/Engine.h"
#include <iostream>
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
    debugMode = false;
    turnCount = 0;
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
        chrono::high_resolution_clock::time_point startTime, endTime;
        Position compMove, bestCompMove;
        int i, bestDepth, bestUtility;

        if (debugMode) {
            startTime = chrono::high_resolution_clock::now();
        }

        Minimax::timeRemaining = chrono::duration<double>(TIME_LIMIT);
        Minimax::resetTransTable();
        compMove = Minimax::search(true, state, startDepth);
        if (compMove.row == -1 || compMove.col == -1) {
            bestDepth = 0;
            bestUtility = 0;
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

        if (debugMode) {
            endTime = chrono::high_resolution_clock::now();
            cout << "Best depth: " + bestDepth << endl;
            cout << "Best utility: " + bestUtility << endl;
            cout << "Transposition table size: " + Minimax::getTransTableSize() << endl;
            cout << "Minimax run time: "
                 << chrono::duration_cast<chrono::duration<double>>(endTime - startTime).count();
        }

        if (Minimax::random) {
            Minimax::random = ++turnCount < 2;
        }

        return compMove;
    } else {
        return {-1,-1};
    }
}

void Engine::movePlayer(Position pos) {
    vector<Position> successors = state.getSuccessors(false);
    bool validPos = false;
    for (Position successor : successors) {
        if (pos == successor) {
            validPos = true;
            break;
        }
    }
    if (validPos) {
        state.move(false, pos);
    } else {
        throw out_of_range("Invalid position: " + to_string(pos.row) +
                           ", " + to_string(pos.col));
    }
}

bool Engine::terminalState() {
    return state.isTerminal();
}

string Engine::getWinner() {
    return state.getWinner();
}

void Engine::reset() {
    state.reset();
    turnCount = 0;
}

// DEBUGGING FUNCTIONS
Engine::Engine(bool xFirst, double timeLimit, bool debug) : Engine(xFirst, timeLimit) {
    debugMode = debug;
}

string Engine::stateString() {
    string result = "";
    for (int i = 0; i < 8; i++) {
        result += state.toString(i) + "\n";
    }
    return result;
}
