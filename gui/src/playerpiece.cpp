#include "playerpiece.h"
#include "mainwindow.h"

PlayerPiece::PlayerPiece(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    // if it is the human player's turn first
    // set the flags to make this image click and draggable
    // and set their image as the white queen.
    if(GameSettings::playerFirst)
    {
        pixmap = new QPixmap(":/images/assets/queenwhite.png");
        setupTurnTrue();
    } else {
        pixmap = new QPixmap(":/images/assets/queenblack.png");
        setClickAndDragFlags(false);
    }

    GameSettings::isHumanTurn = GameSettings::playerFirst;

    originalX = this->x();
    originalY = this->y();

}

void PlayerPiece::setNewPixmap(QString fileName)
{
    pixmap = new QPixmap(fileName);
}

void PlayerPiece::setClickAndDragFlags(bool b)
{
    setFlag(QGraphicsItem::ItemIsSelectable, b);
    setFlag(QGraphicsItem::ItemIsMovable, b);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, b);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, b);
    setAcceptDrops(b);
}

// move the player to another position
// call this when setting up beginning positions
void PlayerPiece::movePlayerTo(qreal x, qreal y)
{
    setPos(x, y);
//    this->paint(new QPainter, new QStyleOptionGraphicsItem, new QWidget);
//    update(boundingRect());
    originalX = x;
    originalY = y;
    emit playerIsDrawn();
}

void PlayerPiece::setupTurnTrue()
{
    GameSettings::isHumanTurn = true;
    setClickAndDragFlags(true);
}

void PlayerPiece::toOriginalPosition()
{
    setPos(originalX, originalY);
}

qreal PlayerPiece::getOriginalX()
{
    return originalX;
}

qreal PlayerPiece::getOriginalY()
{
    return originalY;
}

void PlayerPiece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    if(GameSettings::isHumanTurn) {
//        setPos(QCursor::pos());
//        update(boundingRect());
//    }
}

void PlayerPiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(GameSettings::isHumanTurn)
    {
        qDebug() << this->x() << " mRE " << this->y();
        QGraphicsItem::mouseReleaseEvent(event);

//        originalX = this->x();
//        originalY = this->y();

        emit positionChanged();
    } else
    {
        toOriginalPosition();
    }
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
}

// private slots

void PlayerPiece::changePosition(QPointF point)
{
    this->setPos(point);
}

