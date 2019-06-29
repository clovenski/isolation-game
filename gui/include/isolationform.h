#ifndef ISOLATIONFORM_H
#define ISOLATIONFORM_H

#include "Engine.h"
#include "mainwindow.h"
#include "playerpiece.h"
#include "aipiece.h"
#include "confirmform.h"

#include <stdlib.h>
#include <QWidget>
#include <QBrush>
#include <QColor>
#include <QGraphicsObject>
#include <QApplication>

namespace Ui {
class IsolationForm;
}

class IsolationForm : public QWidget
{
    Q_OBJECT

public:
    explicit IsolationForm(QWidget *parent = nullptr);
    ~IsolationForm();

    /*
     * Starts the game.
     * All game settings (Who's first, and difficulty) must be checked beforehand.
     */
    void startGame();

private:
    struct BoardSquare
    {
        // the x and y position of the board in the graphical scene
        // from the top left point
        int x;
        int y;

        // pointer to the rectangle that is inside the scene
        QGraphicsRectItem *rect;

        // the position in the game board as well as boardSquares array.
        Position position;

        // whether or not this block has been occupied and blocked
        // starts as true until they've been drawn in visually
        bool blocked = true;

    };
    // array of the board squares, should be 8 x 8 by default
    BoardSquare boardSquares[GameSettings::boardSize]
                            [GameSettings::boardSize];

    Ui::IsolationForm *ui;
    QGraphicsScene *scene;

    /*
     * the pieces on the board that are controlled by
     * the human, and computer players respectively.
     */
    PlayerPiece *humanPlayer;
    AiPiece *aiPiece = new AiPiece();
    Engine *ai;

    // the valid moves of the human player displayed on the scene
    std::vector<QGraphicsEllipseItem *> validSquares;

    // the forms that the player must confirm to either go back or reset board
    ConfirmForm *backForm;
    ConfirmForm *resetForm;

    int turnNumber;

    // deletes the graphical board square
    void deleteBoardSquare(int i, int j);

    // moves both human and computer player into their respective starting positions,
    // based on who is first. First starts top left, second starts bottom right.
    void moveToStartingPositions();

    // setup the default colors of the board that makes it look like solid brown wood.
    void woodBoardColors();

    // change the board colors, before drawing them in.
    void changeBoardColors(QColor color1, QColor color2);

    // draws the graphical board square
    void drawBoardSquares(QBrush brush1, QBrush brush2);

    // returns grid position in columns and rows to standard chess notation
    QString positionToText(int col, int row);

    // gets the computer's move from the ai engine, and moves them there.
    void moveComputer();

    // highlight the current player's turn in yellow, and un-highlight the other.
    void highlightCurrentTurn();

signals:
    // signals to go back to the title screen or reset the game board and AI.
    void back();
    void reset();

private slots:
    // move the player once they've moved their player piece
    // also checks if the move is valid.
    void movePlayer();

    // check if the game is over. If not, continue and highlight next person's turn.
    void checkTerminalState();

    // display the valid moves for the human player on the scene and
    // remove the valid moves for the human player on the scene
    void displayValidMoves();
    void removeValidMoves();

    // slots for the confirmation forms for going back to title, and reseting the board.
    void on_backButton_clicked();
    void on_resetButton_clicked();
    void goBack();
    void goReset();
};

#endif // ISOLATIONFORM_H
