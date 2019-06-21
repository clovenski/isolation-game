#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <QTextBrowser>
#include <QLabel>
#include <QStackedWidget>

class IsolationForm;

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
    Ui::MainWindow *ui;

    // the board's corresponding view and scene
    QGraphicsView *view;
    QGraphicsScene *scene;

    // the form where the isolation game is played
    IsolationForm *isoForm;

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
