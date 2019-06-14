#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "isolationform.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <QTextBrowser>
#include <QLabel>
#include <QStackedWidget>

namespace Ui {
class MainWindow;
static int width;
static int height;

}

namespace GameSettings {
// difficulty settings
static bool easy = false;
static bool medium = false;
static bool hard = false;

// whos turn it is, true being player, false being computer
static bool playerFirst = false;
static bool computerFirst = false;

// scene/board width and height
static const int boardWidth = 560;
static const int boardHeight = 560;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // radio button slots
    // easy, medium, hard, difficulty toggles
    void on_easyRadioButton_toggled(bool checked);
    void on_mediumRadioButton_toggled(bool checked);
    void on_hardRadioButton_toggled(bool checked);

    // toggles whos turn at the start
    void on_computerRadioButton_toggled(bool checked);
    void on_playerRadioButton_toggled(bool checked);

    // start button, checks if options are proper
    // then deletes widgets and starts game
    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;

    // the board's corresponding view and scene
    QGraphicsView *view;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
