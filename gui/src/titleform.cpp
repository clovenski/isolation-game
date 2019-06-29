#include "titleform.h"
#include "ui_titleform.h"
#include "mainwindow.h"
#include "isolationform.h"

TitleForm::TitleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleForm)
{

    // make the ui a fixed size
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
    isoFormCreated = false;

    ui->label_startInfo->setText("");
}

TitleForm::~TitleForm()
{
    delete ui;
}

// private slots

// all three radio buttons can't all be on
// so when one is toggled on, the others are toggled off.
void TitleForm::on_easyRadioButton_toggled(bool checked)
{
    GameSettings::easy = checked;
}

void TitleForm::on_mediumRadioButton_toggled(bool checked)
{
    GameSettings::medium = checked;
}

void TitleForm::on_hardRadioButton_toggled(bool checked)
{
    GameSettings::hard = checked;
}

// computer and player radio buttons can't both be on
// so when one is toggled on, the other is toggled off.
void TitleForm::on_computerRadioButton_toggled(bool checked)
{
    GameSettings::computerFirst = checked;
}

void TitleForm::on_playerRadioButton_toggled(bool checked)
{
    GameSettings::playerFirst = checked;
}

void TitleForm::on_startButton_clicked()
{
    // check if options have been toggled properly
    if((GameSettings::easy ? 1 : 0) +
       (GameSettings::medium ? 1 : 0) +
       (GameSettings::hard ? 1 : 0) == 1 &&
        GameSettings::playerFirst != GameSettings::computerFirst)
    {
        GameSettings::isHumanTurn = GameSettings::playerFirst;

        ui->label_startInfo->setStyleSheet("QLabel { color : black; }");
        ui->label_startInfo->setText("Starting Game.");

        QApplication::processEvents();
        emit startButtonClicked();
    }else
    {
        ui->label_startInfo->setStyleSheet("QLabel { color : red; }");
        ui->label_startInfo->setText("Must Enter Proper Settings.");
    }
}


void TitleForm::closeEvent(QCloseEvent *event)
{
//    if(isoFormCreated)
//        isoForm->done = true;
    QWidget::closeEvent(event);
}
