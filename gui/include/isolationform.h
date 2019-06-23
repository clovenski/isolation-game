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
#include <QEventLoop>

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
    bool done;

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
        // starts as true until they've been drawn in visually
        bool blocked = true;

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

    AiPiece *aiPiece = new AiPiece();
    QEventLoop loop;    // TODO, replace with connects instead, and wait for user input?

    ConfirmForm *backForm;
    ConfirmForm *resetForm;

//    bool isHumanTurn;
    void deleteBoardSquare(int i, int j);
    void moveToStartingPositions();
    void drawBoardSquares(QBrush brush1, QBrush brush2);

signals:
    void back();
    void reset();

public slots:
    void movePlayer();
    void endLoop();


private slots:
    void on_backButton_clicked();
    void on_resetButton_clicked();
    void goBack();
    void goReset();
};

#endif // ISOLATIONFORM_H
