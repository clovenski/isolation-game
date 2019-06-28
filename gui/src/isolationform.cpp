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

    // set the difficulty based on the game settings set in title screen
    // 0 being easy, 1 being medium, 2 being hard.
    int difficulty = 0;
    if(GameSettings::medium)
        difficulty = 1;
    else if(GameSettings::hard)
        difficulty = 2;

    // create the ai for the computer player, set player images,
    // and set their posiitons based on who's first
    if(GameSettings::playerFirst)
    {
        ai = new Engine(false, difficulty);
        aiPiece = new AiPiece();
        humanPlayer->setNewPixmap(":/images/assets/queenwhite.png"); 
        ui->label_player1->setText("Player 1: Human");
        ui->label_player2->setText("Player 2: Computer");
    } else {
        ai = new Engine(true, difficulty);
        aiPiece = new AiPiece();
        humanPlayer->setNewPixmap(":/images/assets/queenblack.png");
        ui->label_player1->setText("Player 1: Computer");
        ui->label_player2->setText("Player 2: Human");
    }

    moveToStartingPositions();

    // add the graphics items to the scene
    scene->addItem(humanPlayer);
    scene->addItem(aiPiece);

    // add the back and reset forms that user must confirm to proceed
    backForm = new ConfirmForm();
    QObject::connect(backForm, SIGNAL(yesClicked()),
                     this, SLOT(goBack()));
    backForm->setLabel("Are you sure you want to go back?");

    resetForm = new ConfirmForm();
    QObject::connect(resetForm, SIGNAL(yesClicked()),
                     this, SLOT(goReset()));
    resetForm->setLabel("Are you sure you want to reset the game?");

    QObject::connect(humanPlayer, SIGNAL(positionChanged()),
                     this, SLOT(movePlayer()));
    QObject::connect(humanPlayer, SIGNAL(mousePressed()),
                     this, SLOT(displayValidMoves()));
    QObject::connect(humanPlayer, SIGNAL(mouseReleased()),
                     this, SLOT(removeValidMoves()));
}

IsolationForm::~IsolationForm()
{
    delete ui;
}

// default colors of the board that makes it look like solid brown wood.
void IsolationForm::woodBoardColors(){
    QColor color1 = QColor();
    color1.setNamedColor("#DEB887");

    // second colour
    QColor color2 = QColor();
    color2.setNamedColor("#ae773f");

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
    drawBoardSquares(brush1, brush2);
}

void IsolationForm::startGame()
{
    ui->textBrowser_moves->append("      Player 1 \t Player 2\n");
    turnNumber = 1;
    if(GameSettings::computerFirst)
        moveComputer(); 
}

void IsolationForm::deleteBoardSquare(int i, int j)
{
    boardSquares[i][j].blocked = true;
    delete boardSquares[i][j].rect;
}

void IsolationForm::moveToStartingPositions()
{
    if(GameSettings::playerFirst)
    {
        humanPlayer->movePlayerTo(0,0);
        aiPiece->setPos(GameSettings::sceneSize - GameSettings::pixelSize,
                        GameSettings::sceneSize - GameSettings::pixelSize);
    } else {
        humanPlayer->movePlayerTo(GameSettings::sceneSize - GameSettings::pixelSize,
                                  GameSettings::sceneSize - GameSettings::pixelSize);
        aiPiece->setPos(0,0);
    }
    humanPlayer->setZValue(1);
    aiPiece->setZValue(1);
    humanPlayer->update();
    aiPiece->update();
}

void IsolationForm::drawBoardSquares(QBrush brush1, QBrush brush2)
{
    bool everyOther = true;
    for(int i = 0; i < GameSettings::boardSize; i++)
    {
        for(int j = 0; j < GameSettings::boardSize; j++)
        {
            if(boardSquares[i][j].blocked)
            {
            if(everyOther)
                {
                    boardSquares[i][j].rect = scene->addRect(boardSquares[i][j].x, boardSquares[i][j].y,
                                   GameSettings::pixelSize, GameSettings::pixelSize,
                                   QPen(brush1.color()), brush1);
                } else {
                    boardSquares[i][j].rect = scene->addRect(boardSquares[i][j].x, boardSquares[i][j].y,
                                   GameSettings::pixelSize, GameSettings::pixelSize,
                                   QPen(brush2.color()), brush2);
                }
            }
            boardSquares[i][j].blocked = false;
            everyOther = !everyOther;
        }
        everyOther = !everyOther;
    }
}

QString IsolationForm::positionToText(int col, int row)
{
    QString str = QString();
    switch(col)
    {
    case 0: str.append('A'); break;
    case 1: str.append('B'); break;
    case 2: str.append('C'); break;
    case 3: str.append('D'); break;
    case 4: str.append('E'); break;
    case 5: str.append('F'); break;
    case 6: str.append('G'); break;
    case 7: str.append('H'); break;
    }
    switch(row)
    {
    case 0: str.append('8'); break;
    case 1: str.append('7'); break;
    case 2: str.append('6'); break;
    case 3: str.append('5'); break;
    case 4: str.append('4'); break;
    case 5: str.append('3'); break;
    case 6: str.append('2'); break;
    case 7: str.append('1'); break;
    }

    return str;
}

void IsolationForm::moveComputer()
{
    if(!ai->terminalState())
    {
        deleteBoardSquare(static_cast<int>(aiPiece->y()) / GameSettings::pixelSize,
                          static_cast<int>(aiPiece->x()) / GameSettings::pixelSize);
        Position aiMove = ai->getCompMove();
        qDebug().noquote() << QString::fromStdString(ai->stateString())
                           << QString::fromStdString("Best depth: " + ai->debugCompMove("depth") + "\n")
                           << QString::fromStdString("Best utility: " + ai->debugCompMove("utility") + "\n")
                           << QString::fromStdString("Table size: " + ai->debugCompMove("table size") + "\n")
                           << QString::fromStdString("Minimax run time: " + ai->debugCompMove("run time") + "\n");
        aiPiece->setPos(aiMove.col * GameSettings::pixelSize,
                        aiMove.row * GameSettings::pixelSize);

        // output the computer's move to the textbox, according to chess notation
        QString str = QString();
        if(GameSettings::computerFirst)
        {
            if(turnNumber <= 9)
                str.append(" ");
            str.append(QString::number(turnNumber) + ".           " );
        } else {
            str.append(" \t ");
        }

        str.append(positionToText(aiMove.col, aiMove.row));
        if(!GameSettings::computerFirst)
        {
            str.append("\n");
            turnNumber++;
        }
        ui->textBrowser_moves->moveCursor(QTextCursor::End);
        ui->textBrowser_moves->insertPlainText(str);
        ui->textBrowser_moves->moveCursor(QTextCursor::End);

        humanPlayer->setupTurnTrue();
        checkTerminalState();
    }
}

void IsolationForm::removeValidMoves()
{
    for(auto square: validSquares)
    {
        delete square;
    }
    validSquares.clear();
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
        humanPlayer->toOriginalPosition();
    } else {
        Position playerMove ={-1,-1};

        // finds the position closest to where the player wants to move based off
        // of the pixels and makes that their movement choice.
        int divider = GameSettings::sceneSize / 8;
        playerMove.row = (humanPlayer->y() + (GameSettings::pixelSize / 2)) / divider;
        playerMove.col = (humanPlayer->x() + (GameSettings::pixelSize / 2)) / divider;

        qDebug().noquote() << "Player's move: "<<playerMove.col << ", "<< playerMove.row;

        // tries out if the position the human decided on is a viable position.
        // if not, return.
        try
        {
            // throws out of range if invalid move
            ai->movePlayer(playerMove);

            // Make original position blocked off, and show it visually
            deleteBoardSquare(static_cast<int>(humanPlayer->getOriginalY()) / GameSettings::pixelSize,
                        static_cast<int>(humanPlayer->getOriginalX()) / GameSettings::pixelSize);

            // Moves the player and changes the original position coordinates.
            humanPlayer->movePlayerTo(boardSquares[playerMove.row][playerMove.col].x,
                                      boardSquares[playerMove.row][playerMove.col].y);

            // output to the textbox the move the human player did.
            QString str = QString();
            if(GameSettings::playerFirst)
            {
                if(turnNumber <= 9)
                    str.append(" ");
                str.append(QString::number(turnNumber) + ".           " );
            } else {
                str.append(" \t ");
            }

            str.append(positionToText(playerMove.col, playerMove.row));
            if(!GameSettings::playerFirst)
            {
                str.append("\n");
                turnNumber++;
            }
            ui->textBrowser_moves->moveCursor(QTextCursor::End);
            ui->textBrowser_moves->insertPlainText(str);
            ui->textBrowser_moves->moveCursor(QTextCursor::End);

            GameSettings::isHumanTurn = false;
            checkTerminalState();
            moveComputer();
        } catch(const std::out_of_range e)
        {
            humanPlayer->toOriginalPosition();
            return;
        }
    }
}

void IsolationForm::displayValidMoves()
{
    auto positions = ai->getChoices();
    QColor color = QColor("#40000000"); // #7032cd32 for green, #AARRGGBB, AA for transparency
    QBrush brush = QBrush(Qt::SolidPattern);
    brush.setColor(color);

    for(auto pos: positions)
    {
        validSquares.push_back(scene->addEllipse(pos.col * GameSettings::pixelSize + 24,
                                                 pos.row * GameSettings::pixelSize + 24,
                                                 GameSettings::pixelSize / 3,
                                                 GameSettings::pixelSize / 3,
                                                 QPen(QColor("transparent")),brush));
    }
}

void IsolationForm::checkTerminalState()
{
    if(ai->terminalState())
    {
        humanPlayer->setClickAndDragFlags(false);
        ui->textBrowser_moves->append(QString::fromStdString(ai->getWinner()) + " won!");
        ui->textBrowser_moves->moveCursor(QTextCursor::End);
    }
}

void IsolationForm::goBack()
{
    emit back();
}

// resets the game with the same settings
void IsolationForm::goReset()
{
    ai->reset();

    // redraw any missing squares
    woodBoardColors();

    // reset the positions of the players
    moveToStartingPositions();

    GameSettings::isHumanTurn = GameSettings::playerFirst;

    humanPlayer->setClickAndDragFlags(true);

    ui->textBrowser_moves->clear();
    startGame();
}

// push buttons

void IsolationForm::on_backButton_clicked()
{
    backForm->show();
}

void IsolationForm::on_resetButton_clicked()
{
    resetForm->show();
}


