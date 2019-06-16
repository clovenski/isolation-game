#include "isolationform.h"
#include "ui_isolationform.h"

IsolationForm::IsolationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IsolationForm)
{
    // make the ui a fixed size
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);

    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0,0,GameSettings::sceneSize,
                                   GameSettings::sceneSize);

    // disable the scrolls bars
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setFixedSize(GameSettings::sceneSize,
                                   GameSettings::sceneSize); // TODO unnecessary?
    // set the colours of the board
    color1 = QColor();
    color1.setNamedColor("#DEB887");
    QBrush brush1 = QBrush(Qt::SolidPattern);
    brush1.setColor(color1);
    // second colour
    color2 = QColor();
    color2.setNamedColor("#F5DEB3");
    QBrush brush2 = QBrush(Qt::SolidPattern);
    brush2.setColor(color2);


    // setup up the coordinates of each board square while
    // setting up the board and draw it's checkered squares
    bool everyOther = true;
    for(int i = 0; i < GameSettings::boardSize; i++)
    {

        for(int j = 0; j < GameSettings::boardSize; j++)
        {
            boardSquares[i][j].x = i * GameSettings::pixelSize;
            boardSquares[i][j].y = j * GameSettings::pixelSize;
            if(everyOther)
            {
                scene->addRect(boardSquares[i][j].x, boardSquares[i][j].y,
                               GameSettings::pixelSize, GameSettings::pixelSize,
                               QPen(color1), brush1);
            } else {
                scene->addRect(boardSquares[i][j].x, boardSquares[i][j].y,
                               GameSettings::pixelSize, GameSettings::pixelSize,
                               QPen(color2), brush2);
            }
            everyOther = !everyOther;
        }
        everyOther = !everyOther;
    }

    // draw in the board squares
//    for(auto &squareRow: boardSquares)
//    {
//        bool everyOther = false;
//        for(auto &squareColumn: squareRow)
//        {
//            if(everyOther)
//            {
//                scene->addRect()
//            } else {
//            }
//        }
//    }

    // add the player's pieces
    humanPlayer = new PlayerPiece(":/images/chess-piece-images/queenwhite.png");
    scene->addItem(humanPlayer);

}

IsolationForm::~IsolationForm()
{
    delete ui;
}
