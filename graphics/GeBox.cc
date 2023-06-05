#include "GeBox.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "../utility/utility.h"

GeBox::GeBox(QRectF const &rect) : IGeGraphic(eGeBoxType), m_rect(rect)
{
    _color = {192, 192, 192};
    // setPos(m_rect.center());
    NOTIFY_MAKE_GRAPHIC();
}

GeBox::GeBox(QPointF const &ptMin, QPointF const &ptMax) : IGeGraphic(eGeBoxType)
{
    _color = {192, 192, 192};
    sindy::extent2Rect(ptMin, ptMax, m_rect);
    // setPos(m_rect.center());
    NOTIFY_MAKE_GRAPHIC();
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

    QColor fillColor = getDrawColor(option->state);

    auto oldPen = painter->pen();
    painter->setPen(QPen(getDrawColor(option->state), getDrawWidth(option->state)));
    painter->drawRect(m_rect);
    painter->setPen(oldPen);
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
