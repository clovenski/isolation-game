#ifndef ISOLATIONGAME_H
#define ISOLATIONGAME_H


#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>

class IsolationGame : public QObject
{
    Q_OBJECT

private:

public:
    explicit IsolationGame(QObject *parent = nullptr);
    void startGame(bool difficulty, bool turn) const;

signals:

public slots:
};

#endif // ISOLATIONGAME_H
