#include "playerpiece.h"

PlayerPiece::PlayerPiece(QString fileName)
{
    this->setPixmap(QPixmap(fileName));

    // set the flags to make this image click and draggable
    setClickAndDragFlags();

    originalX = 100;
    originalY = 100;
    this->setPos(originalX, originalY);
//    QGraphicsItem::setBoundingRegionGranularity(1);


}

void PlayerPiece::setNewPixmap(QString fileName)
{
    this->setPixmap(QPixmap(fileName));
}

void PlayerPiece::setClickAndDragFlags()
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setAcceptDrops(true);

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

