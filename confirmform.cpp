#include "confirmform.h"
#include "ui_confirmform.h"

ConfirmForm::ConfirmForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfirmForm)
{
    ui->setupUi(this);
}

ConfirmForm::~ConfirmForm()
{
    delete ui;
}
