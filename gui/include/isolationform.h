#ifndef ISOLATIONFORM_H
#define ISOLATIONFORM_H

#include "Engine.h"
#include <QWidget>

namespace Ui {
class IsolationForm;
}

class IsolationForm : public QWidget
{
    Q_OBJECT

public:
    explicit IsolationForm(QWidget *parent = nullptr);
    ~IsolationForm();

private:
    Ui::IsolationForm *ui;
};

#endif // ISOLATIONFORM_H
