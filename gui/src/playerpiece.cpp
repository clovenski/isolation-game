#include "playerpiece.h"

PlayerPiece::PlayerPiece(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
//    this->setPixmap(QPixmap(fileName));
//    this->setNewPixmap(fileName);
    // set the flags to make this image click and draggable
    setClickAndDragFlags();
    pixmap = new QPixmap(":/images/assets/queenblack.png");
    originalX = 0;
    originalY = 0;
    this->setPos(originalX, originalY);
//    QGraphicsItem::setBoundingRegionGranularity(1);


}

void PlayerPiece::setNewPixmap(QString fileName)
{
    pixmap = new QPixmap(fileName);
    paint(new QPainter,
          new const QStyleOptionGraphicsItem,
          0);
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
    return QRectF(70,70,70,70);
}

void PlayerPiece::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget)
{
    painter->drawPixmap(70,70,pixmap->width(),pixmap->height(), *pixmap);
    qDebug() << "paint";
//    this->setVisible(true);
//    this->show();
}

//void PlayerPiece::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    qDebug() << event->scenePos().x() << ", " << event->scenePos().y();
//}

// TODO this is never called
void PlayerPiece::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "dE";
}

// TODO returns object to orginal position when dragging
//void PlayerPiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    setPos(event->pos());
//    qDebug() << event->pos().x() << ", " << event->pos().y();

//}

// private slots
void PlayerPiece::changePosition(QPointF point)
{
    this->setPos(point);
}

