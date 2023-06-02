#include "BoxGraphic.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "../utility/utility.h"

BoxGraphic::BoxGraphic(QRectF const &rect) : BasicGraphic(eBoxType), m_rect(rect)
{
    // setPos(m_rect.center());
    NOTIFY_MAKE_GRAPHIC();
}

BoxGraphic::BoxGraphic(QPointF const &ptMin, QPointF const &ptMax) : BasicGraphic(eBoxType)
{
    sindy::extent2Rect(ptMin, ptMax, m_rect);
    // setPos(m_rect.center());
    NOTIFY_MAKE_GRAPHIC();
}

QRectF BoxGraphic::boundingRect() const
{
    return m_rect;
}

QPainterPath BoxGraphic::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void BoxGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = getDrawColor(option->state);

    auto oldPen = painter->pen();
    painter->setPen(QPen(getDrawColor(option->state), getDrawWidth(option->state)));
    painter->drawRect(m_rect);
    painter->setPen(oldPen);
}

void BoxGraphic::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void BoxGraphic::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier)
    {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void BoxGraphic::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
