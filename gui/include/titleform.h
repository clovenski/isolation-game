#ifndef TITLEFORM_H
#define TITLEFORM_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <QTextBrowser>
#include <QLabel>
#include <QStackedWidget>

class IsolationForm;

namespace Ui {
class TitleForm;
}

class TitleForm : public QWidget
{
    Q_OBJECT

public:
    explicit TitleForm(QWidget *parent = nullptr);
    ~TitleForm();

signals:
    void startButtonClicked();
private slots:
    // radio button slots
    // easy, medium, hard, difficulty toggles
    void on_easyRadioButton_toggled(bool checked);
    void on_mediumRadioButton_toggled(bool checked);
    void on_hardRadioButton_toggled(bool checked);

    // toggles whose turn at the start
    void on_computerRadioButton_toggled(bool checked);
    void on_playerRadioButton_toggled(bool checked);

    // start button, checks if options are proper
    // then deletes widgets and starts game
    void on_startButton_clicked();

private:
    Ui::TitleForm *ui;

    // the form where the isolation game is played
    IsolationForm *isoForm;
    bool isoFormCreated;

    void closeEvent(QCloseEvent *);
};

#endif // TITLEFORM_H
