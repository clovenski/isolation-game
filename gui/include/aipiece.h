#ifndef AIPIECE_H
#define AIPIECE_H

#include <QGraphicsItem>
#include <QPainter>

class AiPiece : public QGraphicsItem
{
public:
    AiPiece();
protected:
    // overriding pure virtual functions
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;
private:
    QPixmap *pixmap;
};

#endif // AIPIECE_H
