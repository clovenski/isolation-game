#ifndef ISOLATIONFORM_H
#define ISOLATIONFORM_H

#include "Engine.h"
#include "mainwindow.h"
#include "playerpiece.h"

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
private:
    Ui::IsolationForm *ui;
    QGraphicsScene *scene;

    // TODO use struct, position, in engine?
    struct BoardSquare
    {
        // the x and y position of the board from the top left point
        int x;
        int y;

        // the position in the game board as well as boardSquares array.
        Position position;

        // whether or not this block has been occupied and blocked
        bool blocked = false;

    };
    BoardSquare boardSquares[GameSettings::boardSize][GameSettings::boardSize];
    PlayerPiece *humanPlayer;
    PlayerPiece *computerPlayer;

signals:

public slots:


};

#endif // ISOLATIONFORM_H
