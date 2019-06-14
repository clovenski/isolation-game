#pragma once

#include <string>
#include <unordered_map>
#include <cstdlib>
#include <chrono>
#include <string>
#include "State.h"

class Minimax {
    private:
        static Position action;
        static int maxDepth;
        static std::unordered_map<int, std::vector<Position>> optimalMoves;
        static std::unordered_map<std::string, Position> transpositionTable;
        static int oldMaxUtil;
        static int maxUtility;
        static bool earlyStop;
        static std::chrono::high_resolution_clock::time_point startTime;
        static int maxValue(bool, State, int, int, int);
        static int minValue(bool, State, int, int, int);
        static void updateOptimals(int, Position);
        static void putBestFirst(std::vector<Position> &, Position);
    public:
        static bool random;
        static double earlyStopLimit;
        static std::chrono::duration<double> timeRemaining;
        static void resetTransTable();
        static Position search(bool, State, int);
        static int getMaxUtility();
        static int getTransTableSize();
};