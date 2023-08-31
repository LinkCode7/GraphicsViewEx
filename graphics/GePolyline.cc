#include "GePolyline.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
// #include <algorithm>
#include <iostream>

#include "../utility/utility.h"

GePolyline::GePolyline() : IGePointSet(eGePolylineType), GraphicsSymbol(eUnknownSymbol)
{
    _color = {255, 255, 255};
}

GePolyline::GePolyline(std::initializer_list<QPointF> const &pts, GraphicsSymbol::Type type)
    : IGePointSet(pts, eGePolylineType), GraphicsSymbol(type)
{
    _color = {255, 255, 255};
}

void GePolyline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(getDrawColor(option->state), getDrawWidth(option->state)));

    painter->drawPolyline(_points.data(), _points.size());

    if (_symbol != ePointsIndex)
        return;

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

void GePolyline::setLastPt(const QPointF &ptNow)
{
    auto size = _points.size();
    if (size == 0)
        return;
    else if (size == 1)
        _points.emplace_back(ptNow);
    else
        _points[_points.size() - 1] = ptNow;
}

bool GePolyline::getFirstPt(QPointF &pt) const
{
    if (_points.size() == 0)
        return false;
    pt = _points[0];
    return true;
}

void GePolyline::closePolyline(const QPointF &point)
{
    auto size = _points.size();
    if (size <= 3)
        return;

    if (!checkCross(point))
    {
        // addPoint(point);
        _points[_points.size() - 1] = _points[0];
    }
}

bool GePolyline::checkCross(const QPointF &point)
{
    if (_points.size() <= 2)
        return false;

    QPointF beginPt = _points[0];
    QPointF lastPt  = _points[_points.size() - 2];
    for (int i = 1; i < _points.size() - 1; ++i)
    {
        QPointF endPt = _points[i];
        if (collisionDect(lastPt, point, beginPt, endPt))
        {
            if (lastPt == endPt)
            {
                if (onLineDect(point, beginPt, endPt))
                    return true;
            }
            else if (crossDect(lastPt, point, beginPt, endPt))
            {
                return true;
            }
        }
        beginPt = endPt;
        continue;
    }
    return false;
}

bool GePolyline::collisionDect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4)
{
    double x1max = std::max(p1.x(), p2.x());
    double x1min = std::min(p1.x(), p2.x());
    double y1max = std::max(p1.y(), p2.y());
    double y1min = std::min(p1.y(), p2.y());

    double x2max = std::max(p3.x(), p4.x());
    double x2min = std::min(p3.x(), p4.x());
    double y2max = std::max(p3.y(), p4.y());
    double y2min = std::min(p3.y(), p4.y());

    bool checkX = false;
    bool checkY = false;

    if ((x1max >= x2min && x1max <= x2max) || (x1min >= x2min && x1min <= x2max) || (x2max >= x1min && x2max <= x1max) ||
        (x2max >= x1min && x2max <= x1max))
        checkX = true;
    if ((y1max > y2min && y1max < y2max) || (y1min >= y2min && y1min <= y2max) || (y2max >= y1min && y2max <= y1max) ||
        (y2max > y1min && y2max < y1max))
        checkY = true;

    return checkX && checkY;
}
bool GePolyline::crossDect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4)
{
    auto   p1p3  = QPointF(p3.x() - p1.x(), p3.y() - p1.y());
    auto   p1p2  = QPointF(p2.x() - p1.x(), p2.y() - p1.y());
    auto   p1p4  = QPointF(p4.x() - p1.x(), p4.y() - p1.y());
    double left  = p1p3.x() * p1p2.y() - p1p3.y() * p1p2.x();
    double right = p1p4.x() * p1p2.y() - p1p4.y() * p1p2.x();
    double res1  = left * right;

    auto p3p1   = QPointF(p1.x() - p3.x(), p1.y() - p3.y());
    auto p3p4   = QPointF(p4.x() - p3.x(), p4.y() - p3.y());
    auto p3p2   = QPointF(p2.x() - p3.x(), p2.y() - p3.y());
    left        = p3p1.x() * p3p4.y() - p3p1.y() * p3p4.x();
    right       = p3p2.x() * p3p4.y() - p3p2.y() * p3p4.x();
    double res2 = left * right;

    return (res1 < 0 && res2 < 0) ? true : false;
}
bool GePolyline::onLineDect(const QPointF &p, const QPointF &p1, const QPointF &p2)
{
    auto pXpl = QPointF(p1.x() - p2.x(), p1.y() - p2.y());
    auto pXp2 = QPointF(p.x() - p2.x(), p.y() - p2.y());

    double result = pXpl.x() * pXp2.y() - pXpl.y() * pXp2.x();

    auto r1 = result / sqrt(pXpl.x() * pXpl.x() + pXpl.y() * pXpl.y());
    auto r2 = r1 / sqrt(pXp2.x() * pXp2.x() + pXp2.y() * pXp2.y());

    double left   = std::min(p1.x(), p2.x());
    double right  = std::max(p1.x(), p2.x());
    double top    = std::max(p1.y(), p2.y());
    double bottom = std::min(p1.y(), p2.y());

    if (p.x() >= left && p.x() <= right && p.y() >= bottom && p.y() <= top)
        return r2 < 0.05 ? true : false;
    return false;
}
