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
        /* Construct a new engine.
         *      bool - whether or not player X (computer) goes first
         *      double - time limit for AI to think (in seconds)
         *          ie. 20.0 for 20 seconds
         */ 
        Engine(bool, double);
        /* Get a list of the possible moves for the player. */
        std::vector<Position> getChoices();
        /* Allow the AI to think and make a move, returning the
         * position of the move the AI made.
         */
        Position getCompMove();
        /* Process the player's move with the given position. */
        void movePlayer(Position);
        /* Return whether or not the game state is a terminal state.
         * ie. the game is over
         */
        bool terminalState();
        /* Return the winner: 'X', 'O' or 'None' */
        std::string getWinner();
        /* Reset the game state to its initial state. */
        void reset();
};
