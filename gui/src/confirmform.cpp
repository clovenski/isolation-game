#include "confirmform.h"
#include "ui_confirmform.h"

ConfirmForm::ConfirmForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfirmForm)
{
    ui->setupUi(this);
    ui->label->setAlignment(Qt::AlignCenter);
}

ConfirmForm::~ConfirmForm()
{
    delete ui;
}

void ConfirmForm::setLabel(QString str)
{
    ui->label->setText(str);
}

void ConfirmForm::on_yesButton_clicked()
{
    emit yesClicked();
}

void ConfirmForm::on_noButton_clicked()
{
    this->close();
}
