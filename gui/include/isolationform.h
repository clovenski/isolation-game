#ifndef ISOLATIONFORM_H
#define ISOLATIONFORM_H

#include "Engine.h"
#include "mainwindow.h"
#include "playerpiece.h"

#include <stdlib.h>
#include <QWidget>
#include <QBrush>
#include <QColor>

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
private:
    Ui::IsolationForm *ui;
    QGraphicsScene *scene;

    // TODO use struct, position, in engine?
    struct BoardSquare
    {
        // the x and y position of the board from the top left point
        int x;
        int y;

        // pointer to the rectangle that is inside the scene
        QGraphicsRectItem *rect;

        // the position in the game board as well as boardSquares array.
        Position position;

        // whether or not this block has been occupied and blocked
        bool blocked = false;

    };
    // array of the board squares, should be 8 x 8 by default
    BoardSquare boardSquares[GameSettings::boardSize]
                            [GameSettings::boardSize];

    /*
     * the pieces on the board that are controlled by
     * the human, and computer players respectively.
     */
    PlayerPiece *humanPlayer;
    PlayerPiece *computerPlayer;

    Engine *ai;

//    bool isHumanTurn;

signals:

public slots:
    void movePlayer();

};

#endif // ISOLATIONFORM_H
