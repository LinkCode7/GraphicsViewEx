#include "GeSymbolPoint.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "../utility/utility.h"
#include "qline.h"

#define DOT_DIAMETER 40
#define DOT_RADIUS 20

GeSymbolPoint::GeSymbolPoint(QPointF const &pt, GraphicsSymbol::Type type)
    : IGePoint(pt, eGeSymbolPointType), GraphicsSymbol(type)
{
    _color = {255, 0, 0};
}

QRectF GeSymbolPoint::boundingRect() const
{
    QRectF rect;
    sindy::point2Rect(point().x(), point().y(), rect, DOT_RADIUS);
    return rect;
}

QPainterPath GeSymbolPoint::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void GeSymbolPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch (_symbol)
    {
        case GeSymbolPoint::eAimPoint:
        {
            painter->setPen(QPen(getDrawColor(option->state), getDrawWidth(option->state)));
            painter->drawEllipse(point().x() - DOT_RADIUS * 0.5, point().y() - DOT_RADIUS * 0.5, DOT_RADIUS, DOT_RADIUS);

            QLineF line(0 - DOT_RADIUS, 0, 0 + DOT_RADIUS, 0);

            painter->save();
            painter->setTransform(QTransform::fromTranslate(point().x(), point().y()), true);
            painter->drawLine(line);

            QTransform transform;
            transform.rotate(-90);
            painter->drawLine(transform.map(line));

            painter->restore();

            // painter->setPen(QPen(Qt::black, 0));
            break;
        }
        default:
            break;
    }
}

void GeSymbolPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void GeSymbolPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void GeSymbolPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
