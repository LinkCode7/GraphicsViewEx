#include "BoxGraphic.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "../GraphicUtility.h"

BoxGraphic::BoxGraphic(QRectF const &rect) : m_rect(rect)
{
    // setPos(m_rect.center());
}

BoxGraphic::BoxGraphic(double minX, double minY, double maxX, double maxY)
{
    sindy::extent2Rect(minX, minY, maxX, maxY, m_rect);
    // setPos(m_rect.center());
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

    auto pen = painter->pen();
    painter->setPen(fillColor);
    painter->drawRect(m_rect);
    painter->setPen(pen);
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
