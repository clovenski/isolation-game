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

    isoFormCreated = false;
    titleForm = new TitleForm();
    QObject::connect(titleForm, SIGNAL(startButtonClicked()),
                 this, SLOT(toIsolationForm()));
    stackedWidget->addWidget(titleForm);

//    QObject::connect(isoForm, SIGNAL(back()), this, SLOT(toTitleForm()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// private slots

void MainWindow::toTitleForm()
{
    for(int i = 0; i < stackedWidget->count(); i++)
    {
        QWidget* widget = stackedWidget->widget(i);
        stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
//    isoForm->deleteLater();
    isoFormCreated = false;
    titleForm = new TitleForm(); // possible memory leak?
    QObject::connect(titleForm, SIGNAL(startButtonClicked()),
                 this, SLOT(toIsolationForm()));
    stackedWidget->addWidget(titleForm);
    stackedWidget->setCurrentWidget(titleForm);
}

void MainWindow::toIsolationForm()
{
//    for(int i = 0; i < stackedWidget->count(); i++)
//    {
//        QWidget* widget = stackedWidget->widget(i);
//        stackedWidget->removeWidget(widget);
//        widget->deleteLater();
//    }
    isoFormCreated = true;
    isoForm = new IsolationForm();
    QObject::connect(isoForm, SIGNAL(back()), this, SLOT(toTitleForm()));
    QObject::connect(this, SIGNAL(closed()), isoForm, SLOT(endLoop()));
    stackedWidget->addWidget(isoForm);
    stackedWidget->setCurrentWidget(isoForm);
    isoForm->startGame();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit closed();
    // does not work when using back button, having created mult. forms.
    if(isoFormCreated)
        isoForm->done = true;
    QWidget::closeEvent(event);
}
