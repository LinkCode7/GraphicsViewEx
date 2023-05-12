#include "BasicGraphic.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

BasicGraphic::BasicGraphic() : m_color{0, 255, 0}
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF BasicGraphic::boundingRect() const
{
    return {};
}

QPainterPath BasicGraphic::shape() const
{
    return QGraphicsItem::shape();
}

void BasicGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void BasicGraphic::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void BasicGraphic::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void BasicGraphic::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
