#include "../include/Minimax.h"
#include <climits>
#include <stdexcept>
#include <algorithm>
using namespace std;

unordered_map<string, Position> initTransTable() {
    unordered_map<string, Position> table;
    table.reserve(2000000);
    return table;
}

Position Minimax::action;
int Minimax::maxDepth;
unordered_map<int, vector<Position>> Minimax::optimalMoves;
unordered_map<string, Position> Minimax::transpositionTable = initTransTable();
int Minimax::oldMaxUtil;
int Minimax::maxUtility;
bool Minimax::earlyStop;
chrono::high_resolution_clock::time_point Minimax::startTime;
bool Minimax::random = true;
chrono::duration<double> Minimax::timeRemaining;
double Minimax::earlyStopLimit = 1.8;

void Minimax::resetTransTable() {
    transpositionTable.clear();
}

Position Minimax::search(bool forAgentX, State state, int maxDepth) {
    Minimax::maxDepth = maxDepth;
    Minimax::action = {-1, -1};
    if (Minimax::random) {
        optimalMoves.clear();
    }
    Minimax::oldMaxUtil = Minimax::maxUtility;
    Minimax::maxUtility = INT_MIN;
    srand(time(NULL));
    Minimax::earlyStop = false;
    Minimax::startTime = chrono::high_resolution_clock::now();

    Minimax::maxUtility = maxValue(forAgentX, state, INT_MIN, INT_MAX, 0);

    if (Minimax::earlyStop) {
        Minimax::maxUtility = Minimax::oldMaxUtil;
        return {-1, -1};
    }

    if (Minimax::random) {
        int size;
        try {
            size = Minimax::optimalMoves.at(Minimax::maxUtility).size();
            return Minimax::optimalMoves.at(Minimax::maxUtility).at(rand() % size);
        } catch (out_of_range& e) {
            return Minimax::action;
        }
    } else {
        return Minimax::action;
    }
}

void Minimax::updateOptimals(int v, Position move) {
    vector<Position> moves;
    if (v > Minimax::maxUtility) {
        Minimax::optimalMoves.clear();
        moves.push_back(move);
        Minimax::optimalMoves[v] = moves;
        Minimax::maxUtility = v;
    } else {
        try {
            Minimax::optimalMoves.at(v).push_back(move);
        } catch (out_of_range& e) {
            moves.push_back(move);
            Minimax::optimalMoves[v] = moves;
            Minimax::maxUtility = v;
        }
    }

}

void Minimax::putBestFirst(vector<Position> &successors, Position bestSuccessor) {
    vector<Position>::iterator pos = find(successors.begin(), successors.end(), bestSuccessor);
    successors.erase(pos);
    successors.insert(successors.begin(), bestSuccessor);
}

int Minimax::maxValue(bool forAgentX, State state, int alpha, int beta, int depth) {
    Position oldPos, bestSuccessor, bestLocalAction = {-1,-1};
    int v, oldV, minResult, terminalUtil, bestLocalUtility;
    vector<Position> successors;
    string stateString;
    chrono::duration<double> timeElapsed
        = chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now() - Minimax::startTime);
    Minimax::timeRemaining -= timeElapsed;
    Minimax::startTime = chrono::high_resolution_clock::now();

    if (Minimax::timeRemaining.count() <= Minimax::earlyStopLimit) {
        Minimax::earlyStop = true;
        return INT_MAX;
    }

    if (state.isTerminal() || depth == maxDepth) {
        terminalUtil = state.getUtility();
        if (terminalUtil == INT_MAX) {
            return terminalUtil - (depth - 1);
        } else if (terminalUtil == INT_MIN) {
            return terminalUtil + (depth - 1);
        } else {
            return terminalUtil;
        }
    }

    v = INT_MIN;
    if (forAgentX) {
        oldPos.row = state.getXPos().row;
        oldPos.col = state.getXPos().col;
    } else {
        oldPos.row = state.getOPos().row;
        oldPos.col = state.getOPos().col;
    }
    successors = state.getSuccessors(forAgentX);
    stateString = state.toString() + "M";
    try {
        bestSuccessor = transpositionTable.at(stateString);
        putBestFirst(successors, bestSuccessor);
    } catch (out_of_range& e) {
        bestSuccessor.row = -1;
        bestSuccessor.col = -1;
    }
    bestLocalUtility = v;
    bestLocalAction.row = successors[0].row;
    bestLocalAction.col = successors[0].col;
    for (Position move : successors) {
        oldV = v;
        state.move(forAgentX, move);
        minResult = minValue(forAgentX, state, alpha, beta, depth + 1);
        if (Minimax::earlyStop) {
            state.revert(forAgentX, oldPos);
            return 0;
        }
        v = max(v, minResult);
        if (bestLocalUtility < v) {
            bestLocalUtility = v;
            bestLocalAction.row = move.row;
            bestLocalAction.col = move.col;
        }
        if (depth == 0 && v > oldV) {
            Minimax::action.row = move.row;
            Minimax::action.col = move.col;
        }
        if (Minimax::random && depth == 0 && oldV <= minResult) {
            updateOptimals(v, move);
        }
        state.revert(forAgentX, oldPos);
        if (v >= beta) {
            if (bestLocalAction.row != bestSuccessor.row && bestLocalAction.col != bestSuccessor.col) {
                transpositionTable[stateString] = bestLocalAction;
            }
            return v;
        }
        alpha = max(alpha, v);
    }

    if (bestLocalAction.row != bestSuccessor.row && bestLocalAction.col != bestSuccessor.col) {
        transpositionTable[stateString] = bestLocalAction;
    }

    return v;
}

int Minimax::minValue(bool forAgentX, State state, int alpha, int beta, int depth) {
    Position oldPos, bestSuccessor, bestLocalAction;
    int v, terminalUtil, bestLocalUtility;
    vector<Position> successors;
    string stateString;
    chrono::duration<double> timeElapsed
        = chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now() - Minimax::startTime);
    Minimax::timeRemaining -= timeElapsed;
    Minimax::startTime = chrono::high_resolution_clock::now();

    if (Minimax::timeRemaining.count() <= Minimax::earlyStopLimit) {
        Minimax::earlyStop = true;
        return INT_MIN;
    }

    if (state.isTerminal() || depth == maxDepth) {
        terminalUtil = state.getUtility();
        if (terminalUtil == INT_MAX) {
            return terminalUtil - (depth - 1);
        } else if (terminalUtil == INT_MIN) {
            return terminalUtil + (depth - 1);
        } else {
            return terminalUtil;
        }
    }

    v = INT_MAX;
    if (!forAgentX) {
        oldPos.row = state.getXPos().row;
        oldPos.col = state.getXPos().col;
    } else {
        oldPos.row = state.getOPos().row;
        oldPos.col = state.getOPos().col;
    }
    successors = state.getSuccessors(!forAgentX);
    stateString = state.toString() + "m";
    try {
        bestSuccessor = transpositionTable.at(stateString);
        putBestFirst(successors, bestSuccessor);
    } catch (out_of_range& e) {
        bestSuccessor.row = -1;
        bestSuccessor.col = -1;
    }
    bestLocalUtility = v;
    bestLocalAction.row = successors[0].row;
    bestLocalAction.col = successors[0].col;
    for (Position move : successors) {
        state.move(!forAgentX, move);
        v = min(v, maxValue(forAgentX, state, alpha, beta, depth + 1));
        if (Minimax::earlyStop) {
            state.revert(!forAgentX, oldPos);
            return 0;
        }
        if (bestLocalUtility > v) {
            bestLocalUtility = v;
            bestLocalAction.row = move.row;
            bestLocalAction.col = move.col;
        }
        state.revert(!forAgentX, oldPos);
        if (v <= alpha) {
            if (bestLocalAction.row != bestSuccessor.row && bestLocalAction.col != bestSuccessor.col) {
                transpositionTable[stateString] = bestLocalAction;
            }
            return v;
        }
        beta = min(beta, v);
    }

    if (bestLocalAction.row != bestSuccessor.row && bestLocalAction.col != bestSuccessor.col) {
        transpositionTable[stateString] = bestLocalAction;
    }

    return v;
}

int Minimax::getMaxUtility() {
    return Minimax::maxUtility;
}

int Minimax::getTransTableSize() {
    return transpositionTable.size();
}
