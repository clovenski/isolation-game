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

    // make the ui a fixed size
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
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
    // disconnect everything
//    this->disconnect();
    isoFormCreated = false;
    delete titleForm;
    titleForm = new TitleForm();
    QObject::connect(titleForm, SIGNAL(startButtonClicked()),
                     this, SLOT(toIsolationForm()));

    stackedWidget->addWidget(titleForm);
    stackedWidget->setCurrentWidget(titleForm);
    qDebug() << "To title form";
    stackedWidget->removeWidget(isoForm);
    //    delete isoForm;
}

void MainWindow::toIsolationForm()
{
    //disconnect everything
//    this->disconnect();
    delete isoForm;
    isoForm = new IsolationForm();

    isoFormCreated = true;  // TODO do we need this?
    stackedWidget->addWidget(isoForm);
    stackedWidget->setCurrentWidget(isoForm);
    isoForm->startGame();
    QObject::connect(isoForm, SIGNAL(back()), this, SLOT(toTitleForm()));
    qDebug() << "To isolation form.";
    stackedWidget->removeWidget(titleForm);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isoFormCreated)
        isoForm->done = true;
    QWidget::closeEvent(event);
}
