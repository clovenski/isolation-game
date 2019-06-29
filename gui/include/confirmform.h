#ifndef CONFIRMFORM_H
#define CONFIRMFORM_H

#include <QWidget>

namespace Ui {
class ConfirmForm;
}

class ConfirmForm : public QWidget
{
    Q_OBJECT

public:
    explicit ConfirmForm(QWidget *parent = nullptr);
    ~ConfirmForm();
    void setLabel(QString str);

signals:
    void yesClicked();

private slots:
    void on_yesButton_clicked();
    void on_noButton_clicked();

private:
    Ui::ConfirmForm *ui;
};

#endif // CONFIRMFORM_H
