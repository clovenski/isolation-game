#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <QTextBrowser>
#include <QLabel>
#include <QStackedWidget>

class IsolationForm;
class TitleForm;

namespace Ui {
class MainWindow;
static const int width = 800;
static const int height = 600;
}

namespace GameSettings {
// difficulty settings
extern bool easy;
extern bool medium;
extern bool hard;

// whos turn it is at the start
extern bool playerFirst;
extern bool computerFirst;

// who's turn it is currently in the game
extern bool isHumanTurn;

// scene width and height, must be divisble by boardSize for pixelSize
const int sceneSize = 560;

// boardSize x boardSize board, 8 x 8 in this case
const int boardSize = 8;

// the pixel x pixel size of the squares in the board
const int pixelSize = sceneSize / boardSize;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void closed();

private slots:
    // go to the respective form
    void toTitleForm();
    void toIsolationForm();

private:
    Ui::MainWindow *ui;

    QStackedWidget *stackedWidget;

    // the form where the main title screen is
    TitleForm *titleForm;

    // the form where the isolation game is played
    IsolationForm *isoForm;

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
