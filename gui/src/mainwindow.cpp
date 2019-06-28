#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "isolationform.h"
#include "titleform.h"

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

    // make the ui a fixed size, and remove context help button
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);

    stackedWidget = new QStackedWidget();
    this->setCentralWidget(stackedWidget);

    toTitleForm();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// private slots

void MainWindow::toTitleForm()
{
    GameSettings::easy = false;
    GameSettings::medium = false;
    GameSettings::hard = false;
    GameSettings::playerFirst = false;
    GameSettings::computerFirst = false;
    GameSettings::isHumanTurn = false;

    // delte the widgets that we no longer need
    for(int i = 0; i < stackedWidget->count(); i++)
    {
        QWidget* widget = stackedWidget->widget(i);
        stackedWidget->removeWidget(widget);
        if(widget != nullptr)
            delete widget;
    }
    titleForm = new TitleForm(); // possible memory leak?
    QObject::connect(titleForm, SIGNAL(startButtonClicked()),
                 this, SLOT(toIsolationForm()));
    stackedWidget->addWidget(titleForm);
    stackedWidget->setCurrentWidget(titleForm);
}

void MainWindow::toIsolationForm()
{
    for(int i = 0; i < stackedWidget->count(); i++)
    {
        QWidget* widget = stackedWidget->widget(i);
        stackedWidget->removeWidget(widget);
        if(widget != nullptr)
            delete widget;
    }
    isoForm = new IsolationForm();
    QObject::connect(isoForm, SIGNAL(back()), this, SLOT(toTitleForm()));
    stackedWidget->addWidget(isoForm);
    stackedWidget->setCurrentWidget(isoForm);
    isoForm->startGame();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit closed();
    QWidget::closeEvent(event);
    QCoreApplication::quit();
}
