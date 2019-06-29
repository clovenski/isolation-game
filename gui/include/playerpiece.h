#ifndef PLAYERPIECE_H
#define PLAYERPIECE_H

/*
 * Class PlayerPiece
 *
 * Should be the same size as the board squares.
 * Contains everything needed for the piece that represents the
 * human player and computer player.
 */

#include <QGraphicsObject>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>

class PlayerPiece : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit PlayerPiece(QGraphicsItem *parent = 0);

    // set a new image for this object
    void setNewPixmap(QString fileName);

    // set the flags to make this object click and draggable
    void setClickAndDragFlags(bool b);

    // call this when setting up beginning positions
    // put the player piece is it's new position
    void movePlayerTo(qreal x, qreal y);

    // Make it so that it is the human's turn now.
    // This makes player piece image click and draggable,
    // and GameSettings::isHumanTurn is set to true.
    void setupTurnTrue();

    // call if the playerpiece went to a non viable position
    void toOriginalPosition();

    // get the original coordinates, original x and y shouldn't
    // be overwritten by anything but playerpiece
    qreal getOriginalX();
    qreal getOriginalY();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    // overriding pure virtual functions
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;
signals:
    void positionChanged();
    void playerIsDrawn();
    void mousePressed();
    void mouseReleased();

private slots:
    void changePosition(QPointF); // TODO am i using this?

private:
    QPixmap *pixmap;
    qreal originalX;
    qreal originalY;
};

#endif // PLAYERPIECE_H
