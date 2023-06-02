#include "PointGraphic.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "../utility/utility.h"
#include "qline.h"

#define DOT_DIAMETER 40
#define DOT_RADIUS 20

PointGraphic::PointGraphic(QPointF const &pt) : BasicGraphic(ePointType), _pt(pt)
{
    _color = {255, 0, 0};
    NOTIFY_MAKE_GRAPHIC();
}

QRectF PointGraphic::boundingRect() const
{
    QRectF rect;
    sindy::point2Rect(_pt.x(), _pt.y(), rect, DOT_RADIUS);
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
    painter->setPen(QPen(getDrawColor(option->state), getDrawWidth(option->state)));
    painter->drawEllipse(_pt.x() - DOT_RADIUS * 0.5, _pt.y() - DOT_RADIUS * 0.5, DOT_RADIUS, DOT_RADIUS);

    QLineF line(0 - DOT_RADIUS, 0, 0 + DOT_RADIUS, 0);

    painter->save();
    painter->setTransform(QTransform::fromTranslate(_pt.x(), _pt.y()), true);
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
