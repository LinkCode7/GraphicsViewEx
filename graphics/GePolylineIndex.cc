#include "GePolylineIndex.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <iostream>

#include "../utility/utility.h"

GePolylineIndex::GePolylineIndex() : IGePointSet(eGePolylineIndexType)
{
    _color = {196, 196, 196};
    NOTIFY_MAKE_GRAPHIC();
}

GePolylineIndex::GePolylineIndex(std::initializer_list<QPointF> const &list) : IGePointSet(list, eGePolylineIndexType)
{
    _color = {196, 196, 196};
    NOTIFY_MAKE_GRAPHIC();
}

void GePolylineIndex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(getDrawColor(option->state), getDrawWidth(option->state)));
    painter->drawPolyline(_points.data(), _points.size());

    // 绘制索引
    auto size = _points.size();
    if (size < 2)
        return;

    painter->setPen(QPen(getDrawColor(option->state), getDrawWidth(option->state) / 2));

    // 闭合多段线，不绘制最后一个数字
    size = _points[0] == _points[size - 1] ? size - 1 : size;
    for (auto i = 0; i < size; ++i)
    {
        painter->drawText(_points[i], QString::number(i));
    }
}
