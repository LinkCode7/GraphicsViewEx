#include "PointGraphic.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "../GraphicUtility.h"
#include "qline.h"

#define DOT_DIAMETER 40
#define DOT_RADIUS 20

PointGraphic::PointGraphic(double x, double y) : m_pt{x, y}
{
    m_color = {255, 0, 0};
}

QRectF PointGraphic::boundingRect() const
{
    QRectF rect;
    sindy::point2Rect(m_pt.x(), m_pt.y(), rect, DOT_RADIUS);
    return rect;
}

QPainterPath PointGraphic::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void PointGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor fillColor = getDrawColor(option->state);

    painter->setPen(QPen(fillColor, 1));
    painter->drawEllipse(m_pt.x() - DOT_RADIUS * 0.5, m_pt.y() - DOT_RADIUS * 0.5, DOT_RADIUS, DOT_RADIUS);

    QLineF line(0 - DOT_RADIUS, 0, 0 + DOT_RADIUS, 0);

    painter->save();
    painter->setTransform(QTransform::fromTranslate(m_pt.x(), m_pt.y()), true);
    painter->drawLine(line);

    QTransform transform;
    transform.rotate(-90);
    painter->drawLine(transform.map(line));

    painter->restore();

    // painter->setPen(QPen(Qt::black, 0));
}

void PointGraphic::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void PointGraphic::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void PointGraphic::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
