#include "IGeGraphic.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

IGeGraphic::IGeGraphic(ObjectType type) : _color{0, 255, 0}, _type(type)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    _saveFlags.full(true); // 暂时不控制
}

QRectF IGeGraphic::boundingRect() const
{
    return {};
}

QPainterPath IGeGraphic::shape() const
{
    return QGraphicsItem::shape();
}

void IGeGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void IGeGraphic::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void IGeGraphic::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void IGeGraphic::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

QColor IGeGraphic::getDrawColor(QStyle::State state)
{
    auto color = state & QStyle::State_Selected ? _color.darker(230) : _color;
    // if (state & QStyle::State_MouseOver)
    //    color = color.lighter(125);
    return color;
}

double IGeGraphic::getDrawWidth(QStyle::State state)
{
    return state & QStyle::State_Selected ? 2 : 1;
}
