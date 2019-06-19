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

    // disable the scroll bars
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setFixedSize(GameSettings::sceneSize,
                                   GameSettings::sceneSize); // TODO unnecessary?

    // setup up the coordinates of each board square while
    for(int i = 0; i < GameSettings::boardSize; i++)
    {
        for(int j = 0; j < GameSettings::boardSize; j++)
        {
            boardSquares[i][j].x = i * GameSettings::pixelSize;
            boardSquares[i][j].y = j * GameSettings::pixelSize;
        }
    }

    // set the colours of the board
    woodBoardColors();

    // create the ai for the computer player
    ai = new Engine();

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

//    isHumanTurn = GameSettings::playerFirst;

    // add the player's pieces
    humanPlayer = new PlayerPiece();
    qDebug() << GameSettings::playerFirst; //TODO this gets false for when we want true
    if(GameSettings::playerFirst)
        humanPlayer->setNewPixmap(":/images/assets/queenwhite.png");
    else
        humanPlayer->setNewPixmap(":/images/assets/queenblack.png");

    scene->addItem(humanPlayer);

//    bool con1 = QObject::connect(humanPlayer, SIGNAL(xChanged()),
//                        this, SLOT(movePlayer()));
//    bool con2 = QObject::connect(humanPlayer, SIGNAL(yChanged()),
//                        this, SLOT(movePlayer()));
//    if(con1)
//        qDebug() <<"Connect1 worked";
//    if(con2)
//        qDebug() <<"Connect2 worked";

    bool con3 = QObject::connect(humanPlayer, SIGNAL(positionChanged()),
                        this, SLOT(movePlayer()));
    if(con3)
        qDebug() << "Connect3 worked";
}

IsolationForm::~IsolationForm()
{
    delete ui;
}

// default colors of the board that makes it look like wood.
void IsolationForm::woodBoardColors(){
    QColor color1 = QColor();
    color1.setNamedColor("#DEB887");
    // second colour
    QColor color2 = QColor();
    color2.setNamedColor("#F5DEB3");

    changeBoardColors(color1, color2);
}

void IsolationForm::changeBoardColors(QColor color1, QColor color2)
{
    // setup the brushes with regards to their respective colors
    QBrush brush1 = QBrush(Qt::SolidPattern);
    brush1.setColor(color1);
    QBrush brush2 = QBrush(Qt::SolidPattern);
    brush2.setColor(color2);

    // draw the squares in
    bool everyOther = true;
    for(int i = 0; i < GameSettings::boardSize; i++)
    {
        for(int j = 0; j < GameSettings::boardSize; j++)
        {
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
}

// public slots

void IsolationForm::movePlayer()
{

}
