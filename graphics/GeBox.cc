#include "GeBox.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "../utility/sindy_math.h"
#include "../utility/utility.h"

GeBox::GeBox(QRectF const &rect) : IGeGraphic(eGeBoxType), _rect(rect)
{
    _color = {172, 159, 4};
    // setPos(_rect.center());
}

GeBox::GeBox(QPointF const &ptMin, QPointF const &ptMax) : IGeGraphic(eGeBoxType)
{
    _color = {172, 159, 4};
    sindy::extent2Rect(ptMin, ptMax, _rect);
    // setPos(_rect.center());
}

QRectF GeBox::boundingRect() const
{
    return _rect;
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
    painter->drawRect(_rect);
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

void GeBox::list(std::vector<std::pair<std::string, std::string>> &fields) const
{
    IGeGraphic::list(fields);

    std::stringstream ss;
    ss << "LT(" << sindy::simplifyFloat(_rect.left(), 3) << ", " << sindy::simplifyFloat(_rect.top(), 3)
       << "); W:" << sindy::simplifyFloat(_rect.width(), 3) << ", H:" << sindy::simplifyFloat(_rect.height(), 3);

    fields.emplace_back(std::make_pair("rect", ss.str()));
}