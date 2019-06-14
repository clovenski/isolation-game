#include "isolationform.h"
#include "ui_isolationform.h"

IsolationForm::IsolationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IsolationForm)
{
    ui->setupUi(this);
}

IsolationForm::~IsolationForm()
{
    delete ui;
}
