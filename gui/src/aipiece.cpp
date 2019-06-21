#include "aipiece.h"
#include "mainwindow.h"
AiPiece::AiPiece()
{
    if(GameSettings::playerFirst)
    {
        pixmap = new QPixmap(":/images/assets/queenblack.png");
        setPos(GameSettings::sceneSize - GameSettings::pixelSize,
               GameSettings::sceneSize - GameSettings::pixelSize);
    } else {
        pixmap = new QPixmap(":/images/assets/queenwhite.png");
        setPos(0,0);
    }
}

QRectF AiPiece::boundingRect() const
{
    return QRectF(0,0,pixmap->width(),pixmap->height());
}

void AiPiece::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    painter->drawPixmap(0,0,pixmap->width(),pixmap->height(), *pixmap);
}
