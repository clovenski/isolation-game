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
#include <QLayout>

namespace Ui {
class IsolationForm;
}

class IsolationForm : public QWidget
{
    Q_OBJECT

public:
    explicit IsolationForm(QWidget *parent = nullptr);
    ~IsolationForm();
    void woodBoardColors();
    void changeBoardColors(QColor color1, QColor color2);
    void startGame();
//    bool done;

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
    std::vector<QGraphicsRectItem *> validSquares;

    // the forms that the player must confirm to either go back or reset board
    ConfirmForm *backForm;
    ConfirmForm *resetForm;

    int turnNumber;

    // deletes the graphical board square
    void deleteBoardSquare(int i, int j);

    // moves both human and computer player into their starting positions
    void moveToStartingPositions();

    // draws the graphical board square
    void drawBoardSquares(QBrush brush1, QBrush brush2);

    // returns grid position in columns and rows to standard chess notation
    QString positionToText(int col, int row);
    void moveComputer();

signals:
    void back();
    void reset();
    void playerMoved();

public slots:
    void movePlayer();


    // check if the game is over. If not, continue and highlight next person's turn.
    void checkTerminalState();

    // display the valid moves for the human player on the scene
    void displayValidMoves();
    // remove the valid moves for the human player on the scene
    void removeValidMoves();

private slots:
    void on_backButton_clicked();
    void on_resetButton_clicked();
    void goBack();
    void goReset();
};

#endif // ISOLATIONFORM_H
