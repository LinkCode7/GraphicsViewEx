#include "GeSegment.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <iostream>

#include "../utility/utility.h"

GeSegment::GeSegment() : IGePointSet(eGeSegmentType)
{
    _color = {196, 196, 196};
    NOTIFY_MAKE_GRAPHIC();
}

GeSegment::GeSegment(QPointF const &from, QPointF const &to) : IGePointSet({from, to}, eGeSegmentType)
{
    _color = {196, 196, 196};
    NOTIFY_MAKE_GRAPHIC();
}

void GeSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    assert(_points.size() == 2);
    painter->setPen(QPen(getDrawColor(option->state), getDrawWidth(option->state)));
    painter->drawPolyline(_points.data(), _points.size());
}

QPointF GeSegment::beginPoint() const
{
    assert(_points.size() == 2);
    return _points[0];
}
void GeSegment::beginPoint(QPointF const &pt)
{
    assert(_points.size() == 2);
    _points[0] = pt;
}

QPointF GeSegment::endPoint() const
{
    assert(_points.size() == 2);
    return _points[1];
}
void GeSegment::endPoint(QPointF const &pt)
{
    assert(_points.size() == 2);
    _points[1] = pt;
}