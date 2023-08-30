#include "GeBox.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "../utility/utility.h"

GeBox::GeBox(QRectF const &rect) : IGeGraphic(eGeBoxType), m_rect(rect)
{
    _color = {172, 159, 4};
    // setPos(m_rect.center());
}

GeBox::GeBox(QPointF const &ptMin, QPointF const &ptMax) : IGeGraphic(eGeBoxType)
{
    _color = {172, 159, 4};
    sindy::extent2Rect(ptMin, ptMax, m_rect);
    // setPos(m_rect.center());
}

QRectF GeBox::boundingRect() const
{
    return m_rect;
}

QPainterPath GeBox::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void GeBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(getDrawColor(option->state), getDrawWidth(option->state), Qt::DashLine));
    painter->drawRect(m_rect);
}

void GeBox::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void GeBox::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier)
    {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void GeBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
