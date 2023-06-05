#include "GeAim.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "../utility/utility.h"
#include "qline.h"

#define DOT_DIAMETER 40
#define DOT_RADIUS 20

GeAim::GeAim(QPointF const &pt) : IGePoint(pt, eGeAimType)
{
    _color = {255, 0, 0};
    NOTIFY_MAKE_GRAPHIC();
}

QRectF GeAim::boundingRect() const
{
    QRectF rect;
    sindy::point2Rect(_pt.x(), _pt.y(), rect, DOT_RADIUS);
    return rect;
}

QPainterPath GeAim::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void GeAim::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void GeAim::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void GeAim::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void GeAim::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
