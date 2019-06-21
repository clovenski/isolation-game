#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "isolationform.h"

namespace GameSettings
{
bool easy = false;
bool medium = false;
bool hard = false;

bool playerFirst = false;
bool computerFirst = false;

bool isHumanTurn = false;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    // make the ui a fixed size
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// private slots

// all three radio buttons can't all be on
// so when one is toggled on, the others are toggled off.
void MainWindow::on_easyRadioButton_toggled(bool checked)
{
    GameSettings::easy = checked;
}

void MainWindow::on_mediumRadioButton_toggled(bool checked)
{
    GameSettings::medium = checked;
}

void MainWindow::on_hardRadioButton_toggled(bool checked)
{
    GameSettings::hard = checked;
}

// computer and player radio buttons can't both be on
// so when one is toggled on, the other is toggled off.
void MainWindow::on_computerRadioButton_toggled(bool checked)
{
    GameSettings::computerFirst = checked;
}

void MainWindow::on_playerRadioButton_toggled(bool checked)
{
    GameSettings::playerFirst = checked;
}

void MainWindow::on_startButton_clicked()
{
    // check if options have been toggled properly
    if((GameSettings::easy ? 1 : 0) +
       (GameSettings::medium ? 1 : 0) +
       (GameSettings::hard ? 1 : 0) == 1 &&
        GameSettings::playerFirst != GameSettings::computerFirst)
    {
        GameSettings::isHumanTurn = GameSettings::playerFirst;

        QStackedWidget *stackedWidget;
        stackedWidget = new QStackedWidget();
        isoForm = new IsolationForm();
        stackedWidget->addWidget(isoForm);
        this->setCentralWidget(stackedWidget);
        isoForm->startGame();
    }
}
