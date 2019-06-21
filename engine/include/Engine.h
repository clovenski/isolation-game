/*
 * Class Engine
 *
 * To play the isolation game using the AI
 * call either movePlayer or getCompMove first
 * depending on who goes first.
 *
 * To start a new game, call reset().
 * To change the settings call
 * setWhoFirst(bool) and setTimeLimit(double)
 * before starting again.
 *
 * Rows and Columns are in rows and colums,
 * not x and y cartesian coordinates.
 */

#pragma once

#include "Minimax.h"
#include "State.h"

class Engine {
    private:
        State state;
        bool xFirst;
        std::chrono::duration<double> TIME_LIMIT;
        int turnCount;
        int startDepth;
        double earlyStopLimit;
        bool debugMode;
        Position getRandCompMove();
        void processTimeLimit(double);
    public:
        /* Construct a new engine.
         *      bool - whether or not player X (computer) goes first
         *      double - time limit for AI to think (in seconds)
         *          ie. 20.0 for 20 seconds
         *          must be >= 1.0, otherwise defaults to 2.0 seconds
         * 
         * The state of the game is in its initial state
         * with the player that goes first positioned
         * at the top-left, other player at the bottom-right.
         */ 
        Engine(bool xFirst = true, double timeLimit = 2.0);

        /* DEBUGGING FUNCTION, debug mode on when debug = true */
        Engine(bool xFirst, double timeLimit, bool debug);

        /* Set who goes first; true for player X (computer) goes first,
         * false for player O (user) goes first.
         * 
         * This would have an effect on the initial state of the game
         * when the engine is reset using Engine.reset().
         */
        void setWhoFirst(bool);

        /* Set the time limit for the AI to think (in seconds)
         * Must be >= 1.0, otherwise defaults to 2.0 seconds.
         * 
         * This would have an immediate effect on how much time
         * the AI has to think to make a move. Subsequent calls
         * to Engine.getCompMove() would use the new time limit.
         */
        void setTimeLimit(double);

        /* Get a list of all the possible moves for the player
         * at the current state of the game.
         */
        std::vector<Position> getChoices();

        /* Allow the AI to think and make a move, returning the
         * position of the move the AI made.
         * 
         * The first two calls to this function from when the
         * Engine was instantiated or from the last time it was
         * reset, return a random "smart" move, as in the AI makes
         * a move randomly picked from a set of moves it thinks
         * are equally good to one another.
         * 
         * A call to this function changes the state of the game
         * according to the move the AI made, as in the AI moves
         * on the board and its new position is returned.
         * 
         * If the game is in a terminal state as returned by
         * Engine.terminalState(), then nothing is processed
         * in the game state and a Position with -1 as its
         * row and col properties is returned; indicating an
         * invalid call to this function.
         * 
         * If debugMode = true, prints some debugging info to
         * stdout.
         */
        Position getCompMove();

        /* Process the player's move with the given position.
         * 
         * The given position must be a valid choice for the player,
         * as in the given position must be in the list returned by
         * Engine.getChoices(), otherwise an out_of_range exception
         * is thrown.
         *
         * The state of the game will now have the player at the
         * specified position on the board.
         */
        void movePlayer(Position);

        /* Return whether or not the game state is a terminal state.
         * ie. the game is over
         * 
         * True indicates the game is over and Engine.getWinner()
         * will indicate the winner, and false indicates an ongoing
         * game.
         */
        bool terminalState();

        /* Return the winner: 'Computer', 'You' or 'No one' */
        std::string getWinner();

        /* Reset the game state to its initial state.
         *
         * The game state will reset to its initial state with the board
         * clear of any moves made (blocked tiles), the players at their
         * starting positions as indicated by who is set to go first.
         * 
         * After this call, the engine is ready for a new game.
         */
        void reset();

        /* DEBUGGING FUNCTION
         * 
         * Returns a string representing the current state of the board
         * as the engine perceives it.
         * 
         * Use this to check if the engine's game state and the gui board
         * state are synchronized.
         */
        std::string stateString();
};
