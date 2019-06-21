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
            boardSquares[i][j].x = j * GameSettings::pixelSize;
            boardSquares[i][j].y = i * GameSettings::pixelSize;
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
        ai = new Engine(false, 2.0);
        humanPlayer->setNewPixmap(":/images/assets/queenwhite.png");
        humanPlayer->movePlayerTo(0,0);
        aiPiece = new AiPiece();
        aiPiece->setPos(GameSettings::sceneSize - GameSettings::pixelSize,
                        GameSettings::sceneSize - GameSettings::pixelSize);
    } else {
        ai = new Engine(true, 2.0);
        humanPlayer->setNewPixmap(":/images/assets/queenblack.png");
        humanPlayer->movePlayerTo(GameSettings::sceneSize - GameSettings::pixelSize,
                                  GameSettings::sceneSize - GameSettings::pixelSize);
        aiPiece = new AiPiece();
        aiPiece->setPos(0,0);
    }

    // add the graphics items to the scene
    scene->addItem(humanPlayer);
    scene->addItem(aiPiece);

    bool con2 = QObject::connect(humanPlayer, SIGNAL(positionChanged()),
                                 this, SLOT(movePlayer()));
    bool con3 = QObject::connect(humanPlayer, SIGNAL(positionChanged()),
                                 &loop, SLOT(quit()));
    if(con2)
        qDebug() << "Connect2 worked";
    if(con3)
        qDebug() << "Connect3 worked";

    done = false;
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

    while(!ai->terminalState() && !done)
    {
        if(GameSettings::isHumanTurn)
        {
            qDebug() << "foo";
            loop.exec();
        } else {
            qDebug() << "a3";
            deleteBoardSquare(static_cast<int>(aiPiece->y()) / GameSettings::pixelSize,
                              static_cast<int>(aiPiece->x()) / GameSettings::pixelSize);
            aiMove = ai->getCompMove();
            qDebug().noquote() << QString::fromStdString(ai->stateString())
                               << QString::fromStdString("Best depth: " + ai->debugCompMove("depth") + "\n")
                               << QString::fromStdString("Best utility: " + ai->debugCompMove("utility") + "\n")
                               << QString::fromStdString("Table size: " + ai->debugCompMove("table size") + "\n")
                               << QString::fromStdString("Minimax run time: " + ai->debugCompMove("run time") + "\n");
            aiPiece->setPos(aiMove.col * GameSettings::pixelSize,
                            aiMove.row * GameSettings::pixelSize);
//            deleteBoardSquare(aiMove.row, aiMove.col);
            qDebug() << "Computer Move: " << aiMove.col << ", "<< aiMove.row;
            humanPlayer->setupTurnTrue();
        }

//        if(loop == nullptr)
//            return;
    }
    qDebug() << QString::fromStdString(ai->getWinner()); // TODO put in textbrowser
}

void IsolationForm::deleteBoardSquare(int i, int j)
{
    boardSquares[i][j].blocked = true;
    delete boardSquares[i][j].rect;
}

// public slots

void IsolationForm::movePlayer()
{
    // check if the player is out of bounds
    if(humanPlayer->x() > GameSettings::sceneSize
    || humanPlayer->y() > GameSettings::sceneSize
    || humanPlayer->x() < -GameSettings::pixelSize / 2
    || humanPlayer->y() < -GameSettings::pixelSize / 2)
    {
        qDebug() << "Putting to original position";
        humanPlayer->toOriginalPosition();
    } else {
        Position playerMove ={-1,-1};

        // finds the position closest to where the player wants to move based off
        // of the pixels and makes that their movement choice.
        int divider = GameSettings::sceneSize / 8;
        playerMove.row = (humanPlayer->y() + (GameSettings::pixelSize / 2)) / divider;
        playerMove.col = (humanPlayer->x() + (GameSettings::pixelSize / 2)) / divider;

        qDebug().noquote() << "Player's move: "<<playerMove.col << ", "<< playerMove.row;

        // vector of viable choices for the human player, not ai player
//        std::vector<Position> choices = ai->getChoices();

        // tries out if the position the human decided on is a viable position.
        // if not, return.
        try
        {
            ai->movePlayer(playerMove);
            // Make original position blocked off, and show it visually
            deleteBoardSquare(static_cast<int>(humanPlayer->getOriginalY()) / GameSettings::pixelSize,
                        static_cast<int>(humanPlayer->getOriginalX()) / GameSettings::pixelSize);
            qDebug() << "X: " << boardSquares[playerMove.row][playerMove.col].x
                     << " Y: " << boardSquares[playerMove.row][playerMove.col].y;
            // Moves the player and changes the original position coordinates.
            humanPlayer->movePlayerTo(boardSquares[playerMove.row][playerMove.col].x,
                                      boardSquares[playerMove.row][playerMove.col].y);

            GameSettings::isHumanTurn = false;

            loop.exit();
        } catch(const std::out_of_range e)
        {
            qDebug() << "Invalid Move.";
            humanPlayer->toOriginalPosition();
            return;
        }
    }
}
