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

            boardSquares[i][j].position.row = i;
            boardSquares[i][j].position.col = j;
        }
    }

    // set the colours of the board
    woodBoardColors();

    // add the human player's pieces
    humanPlayer = new PlayerPiece();

    // create the ai for the computer player, set player images,
    // and set their posiitons based on who's first
    if(GameSettings::playerFirst)
    {
        ai = new Engine(true, 2.0);
        humanPlayer->setNewPixmap(":/images/assets/queenwhite.png");
        humanPlayer->movePlayerTo(0,0);
    } else {
        ai = new Engine(false, 2.0);
        humanPlayer->setNewPixmap(":/images/assets/queenblack.png");
        humanPlayer->movePlayerTo(GameSettings::sceneSize - GameSettings::pixelSize,
                                  GameSettings::sceneSize - GameSettings::pixelSize);
    }

    scene->addItem(humanPlayer);

    bool con3 = QObject::connect(humanPlayer, SIGNAL(positionChanged()),
                        this, SLOT(movePlayer()));
    if(con3)
        qDebug() << "Connect3 worked";

//    startGame(); // TODO Crashes once start button in mainwindow.ui is pressed
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
                boardSquares[i][j].rect = scene->addRect(boardSquares[i][j].x, boardSquares[i][j].y,
                               GameSettings::pixelSize, GameSettings::pixelSize,
                               QPen(color1), brush1);
            } else {
                boardSquares[i][j].rect = scene->addRect(boardSquares[i][j].x, boardSquares[i][j].y,
                               GameSettings::pixelSize, GameSettings::pixelSize,
                               QPen(color2), brush2);
            }
            everyOther = !everyOther;
        }
        everyOther = !everyOther;
    }
}

void IsolationForm::startGame()
{
    Position aiMove;
    while(!ai->terminalState())
    {
        if(GameSettings::isHumanTurn)
        {
            // wait for player piece to get the human's turn
        } else {
            aiMove = ai->getCompMove();
            qDebug() << "Computer Move: " << aiMove.row << ", "<< aiMove.col;
            GameSettings::isHumanTurn = true;
        }
    }
    qDebug() << QString::fromStdString(ai->getWinner()); // TODO put in textbrowser
}

// public slots

void IsolationForm::movePlayer()
{
    // check if the player is out of bounds
    if(humanPlayer->x() > GameSettings::sceneSize
    || humanPlayer->y() > GameSettings::sceneSize
    || humanPlayer->x() < -GameSettings::pixelSize
    || humanPlayer->y() < -GameSettings::pixelSize)
    {
        qDebug() << "Putting to original position";
        humanPlayer->toOriginalPosition();
    } else {
        // the square closest to the top left corner of the player piece image
        // with regards to the square's top left corner
        int closestX = GameSettings::sceneSize;
        int closestY = GameSettings::sceneSize;

        Position playerMove ={-1,-1};
        // finds the position closest to where the player wants to move based off
        // of the pixelsand makes that their movement choice.
        for(int i = 0; i < GameSettings::boardSize; i++)
        {
            for(int j = 0; j < GameSettings::boardSize; j++)
            {
                if(abs(boardSquares[i][j].x - humanPlayer->x()) < closestX)
                {
                    closestX = static_cast<int>(
                                abs(boardSquares[i][j].x - humanPlayer->x()));
                    playerMove.row = boardSquares[i][j].position.row;
                    qDebug() << "d1";
                }
                if(abs(boardSquares[i][j].y - humanPlayer->y()) < closestY)
                {
                    closestY = static_cast<int>(
                                abs(boardSquares[i][j].y - humanPlayer->y()));
                    playerMove.col = boardSquares[i][j].position.col;
                    qDebug() << "d2";
                }
            }
        }

        // vector of viable choices for the human player, not ai player
        std::vector<Position> choices = ai->getChoices();

        // tries out if the position the human decided on is a viable position.
        // if not, return.
        // TODO, is never valid move
        try
        {
            ai->movePlayer(playerMove);
        } catch(const std::out_of_range e)
        {
            qDebug() << "Invalid Move.";
            humanPlayer->toOriginalPosition();
            return;
        }

        qDebug() << "d3";
        // Make original position blocked off, and show it visually
        boardSquares[static_cast<int>(humanPlayer->getOriginalX()) / GameSettings::pixelSize]
                    [static_cast<int>(humanPlayer->getOriginalY()) / GameSettings::pixelSize]
                    .blocked = true;
        delete boardSquares[static_cast<int>(humanPlayer->getOriginalX()) / GameSettings::pixelSize]
                           [static_cast<int>(humanPlayer->getOriginalY()) / GameSettings::pixelSize]
                           .rect;
        // Moves the player and changes the original position coordinates.
        humanPlayer->movePlayerTo(boardSquares[playerMove.row][playerMove.col].x,
                boardSquares[playerMove.row][playerMove.col].y);

        GameSettings::isHumanTurn = false;
    }
}
