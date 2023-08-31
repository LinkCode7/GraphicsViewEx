#include "GeSquarePoints.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <iostream>

#include "../utility/utility.h"

#define SQUARE_LENGTH_HALF 2

GeSquarePoints::GeSquarePoints() : IGePointSet(eGeSquarePointsType)
{
    _color = {0, 0, 255};
    // setFlags(0);
    setAcceptHoverEvents(false);
}

GeSquarePoints::GeSquarePoints(std::initializer_list<QPointF> const &pts) : IGePointSet(pts, eGeSquarePointsType)
{
    _color = {0, 0, 255};
    // setFlags(0);
    setAcceptHoverEvents(false);
}

void GeSquarePoints::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(_color);
    painter->setBrush(_color);

    QRectF rect;
    for (auto const &pt : _points)
    {
        sindy::point2Rect(pt.x(), pt.y(), rect, SQUARE_LENGTH_HALF);
        painter->drawRect(rect);
    }
}
