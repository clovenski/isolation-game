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

    // create the ai for the computer player
    if(GameSettings::playerFirst)
        ai = new Engine(true, 2.0);
    else
        ai = new Engine(false, 2.0);

    // add the player's pieces
    humanPlayer = new PlayerPiece();

    if(GameSettings::playerFirst)
        humanPlayer->setNewPixmap(":/images/assets/queenwhite.png");
    else
        humanPlayer->setNewPixmap(":/images/assets/queenblack.png");

    scene->addItem(humanPlayer);

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
        // check if the move is inside the scene // TODO check only list of viable positions
        for(int i = 0; i < GameSettings::boardSize; i++)
        {
            for(int j = 0; j < GameSettings::boardSize; j++)
            {
                if(abs(boardSquares[i][j].x - humanPlayer->x()) < closestX)
                {
                    closestX = static_cast<int>(
                                abs(boardSquares[i][j].x - humanPlayer->x()));
                    playerMove.row = boardSquares[i][j].position.row;
                }
                if(abs(boardSquares[i][j].y - humanPlayer->y()) < closestY)
                {
                    closestY = static_cast<int>(
                                abs(boardSquares[i][j].y - humanPlayer->y()));
                    playerMove.col = boardSquares[i][j].position.col;
                }
            }
        }
        // if we were able to find a viable spot, we can move there
        if(closestX != GameSettings::sceneSize
        || closestY != GameSettings::sceneSize)
        {
            humanPlayer->movePlayerTo(boardSquares[playerMove.row][playerMove.col].x,
                    boardSquares[playerMove.row][playerMove.col].y);
        }

        qDebug() << "closest x: " << closestX << ", closest y:" << closestY
                << "Player's move: ("<< playerMove.row <<", "<< playerMove.col << ")";
    }
    // TODO see if the player is allowed to move to a square

    // TODO send computer AI the move
}
