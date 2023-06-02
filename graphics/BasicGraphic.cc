#include "BasicGraphic.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

BasicGraphic::BasicGraphic(ObjectType type) : _color{0, 255, 0}, _type(type)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    _saveFlags.full(true); // 暂时不控制
}

QRectF BasicGraphic::boundingRect() const
{
    return {};
}

QPainterPath BasicGraphic::shape() const
{
    return QGraphicsItem::shape();
}

void BasicGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void BasicGraphic::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void BasicGraphic::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void BasicGraphic::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

QColor BasicGraphic::getDrawColor(QStyle::State state)
{
    auto color = state & QStyle::State_Selected ? _color.darker(300) : _color;
    // if (state & QStyle::State_MouseOver)
    //    color = color.lighter(125);
    return color;
}

double BasicGraphic::getDrawWidth(QStyle::State state)
{
    return state & QStyle::State_Selected ? 2 : 1;
}
