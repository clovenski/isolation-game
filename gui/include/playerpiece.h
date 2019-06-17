#ifndef PLAYERPIECE_H
#define PLAYERPIECE_H

// Class PlayerPiece
//
// Should be the same size as the board squares.
// Contains everything needed for the piece that represents the
// human player and computer player.

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QPointF>

class PlayerPiece : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit PlayerPiece(QString filename);

    // set a new image for this object
    void setNewPixmap(QString fileName);

    // set the flags to make this object click and draggable
    void setClickAndDragFlags();

private:
    QPointF mousePosition;
//    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override; // TODO unsuable?
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    qreal originalX;
    qreal originalY;

signals:

private slots:
    void changePosition(QPointF);
};

#endif // PLAYERPIECE_H
