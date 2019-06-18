#include "playerpiece.h"

PlayerPiece::PlayerPiece(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    // set the flags to make this image click and draggable
    setClickAndDragFlags();

    pixmap = new QPixmap(":/images/assets/queenblack.png");

    originalX = this->x();
    originalY = this->y();
    this->setPos(originalX, originalY);

}

void PlayerPiece::setNewPixmap(QString fileName)
{
    pixmap = new QPixmap(fileName);
//    paint(new QPainter,
//          new const QStyleOptionGraphicsItem,
//          0);
}

void PlayerPiece::setClickAndDragFlags()
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setAcceptDrops(true);

}

// pure virtual functions
QRectF PlayerPiece::boundingRect() const
{
    // pixmap width and height should be the same as GameSettings::pixelSize
    // and should be a square, same as the board squares.
    return QRectF(0,0,pixmap->width(),pixmap->height());
}

void PlayerPiece::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget)
{
    painter->drawPixmap(0,0,pixmap->width(),pixmap->height(), *pixmap);
    qDebug() << "paint";
//    this->setVisible(true);
//    this->show();
}

// TODO this is never called
void PlayerPiece::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "dE";
}

void PlayerPiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << this->x() << " mRE " << this->y();
    QGraphicsItem::mouseReleaseEvent(event);

    originalX = this->x();
    originalY = this->y();
}

// private slots
void PlayerPiece::changePosition(QPointF point)
{
    this->setPos(point);
}

