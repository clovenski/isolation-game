#pragma once

#include "Minimax.h"
#include "State.h"

class Engine {
    private:
        State state;
        bool xFirst;
        std::chrono::duration<double> TIME_LIMIT;
        int startDepth;
        double earlyStopLimit;
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
         * A call to this function changes the state of the game
         * according to the move the AI made, as in the AI moves
         * on the board and its new position is returned.
         * 
         * If the game is in a terminal state as returned by
         * Engine.terminalState(), then nothing is processed
         * in the game state and a Position with -1 as its
         * row and col properties is returned; indicating an
         * invalid call to this function.
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

        /* Return the winner: 'X', 'O' or 'None' to indicate the winner. */
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
