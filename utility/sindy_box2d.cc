#include "sindy_box2d.h"

#include "float.h"

#define MY_EXTENTS_MAX -DBL_MAX
#define MY_EXTENTS_MIN DBL_MAX
#define SINDY_ZERO 0.00001 // 1e-5

bool compareDouble(double value1, double value2, double tol = SINDY_ZERO)
{
    double sub = value1 - value2;
    if (sub < 0)
        sub = -sub;

    if (sub <= tol)
        return true;
    return false;
}

namespace sindy
{
Box2d::Box2d() : _min{MY_EXTENTS_MIN, MY_EXTENTS_MIN}, _max{MY_EXTENTS_MAX, MY_EXTENTS_MAX}
{
}
Box2d::Box2d(Point2d const& pt) : _min(pt), _max(pt)
{
}
Box2d::Box2d(const std::initializer_list<Point2d>& list)
    : _min{MY_EXTENTS_MIN, MY_EXTENTS_MIN}, _max{MY_EXTENTS_MAX, MY_EXTENTS_MAX}
{
    for (const auto& point : list)
        add(point);
}

void Box2d::reset()
{
    _min = {MY_EXTENTS_MIN, MY_EXTENTS_MIN};
    _max = {MY_EXTENTS_MAX, MY_EXTENTS_MAX};
}
void Box2d::reset(Point2d const& pt1, Point2d const& pt2)
{
    _min = pt1;
    _max = pt1;
    add(pt2);
}
void Box2d::set(Point2d const& ptMin, Point2d const& ptMax)
{
    _min = ptMin;
    _max = ptMax;
}

// 建议使用前先判断有效性
bool Box2d::invalid() const
{
    if (compareDouble(_min.x(), MY_EXTENTS_MIN) && compareDouble(_min.y(), MY_EXTENTS_MIN) &&
        compareDouble(_max.x(), MY_EXTENTS_MAX) && compareDouble(_max.y(), MY_EXTENTS_MAX))
        return true;
    return false;
}
void Box2d::add(Point2d const& pt)
{
    if (pt.x() < _min.x())
        _min.x(pt.x());
    if (pt.x() > _max.x())
        _max.x(pt.x());

    if (pt.y() < _min.y())
        _min.y(pt.y());
    if (pt.y() > _max.y())
        _max.y(pt.y());
}
void Box2d::add(Box2d const& ext)
{
    add(ext._min);
    add(ext._max);
}
void Box2d::operator+=(Box2d const& ext)
{
    add(ext._min);
    add(ext._max);
}

bool Box2d::inBox(Point2d const& pt) const
{
    if (pt.x() < _min.x() || pt.x() > _max.x())
        return false;
    if (pt.y() < _min.y() || pt.y() > _max.y())
        return false;
    return true;
}
bool Box2d::inBox(Point2d const& pt, double tol) const
{
    if (pt.x() < _min.x() - tol || pt.x() > _max.x() + tol)
        return false;
    if (pt.y() < _min.y() - tol || pt.y() > _max.y() + tol)
        return false;
    return true;
}
bool Box2d::outBox(Point2d const& pt) const
{
    if (pt.x() < _min.x() || pt.x() > _max.x())
        return true;
    if (pt.y() < _min.y() || pt.y() > _max.y())
        return true;
    return false;
}
bool Box2d::outBox(Point2d const& pt, double tol) const
{
    if (pt.x() < _min.x() - tol || pt.x() > _max.x() + tol)
        return true;
    if (pt.y() < _min.y() - tol || pt.y() > _max.y() + tol)
        return true;
    return false;
}
bool Box2d::outBox(Box2d const& ext) const
{
    if (ext._max.x() < _min.x() || ext._min.x() > _max.x())
        return true;
    if (ext._max.y() < _min.y() || ext._min.y() > _max.y())
        return true;
    return false;
}
bool Box2d::outBox(Box2d const& ext, double tol) const
{
    if (ext._max.x() < _min.x() - tol || ext._min.x() > _max.x() + tol)
        return true;
    if (ext._max.y() < _min.y() - tol || ext._min.y() > _max.y() + tol)
        return true;
    return false;
}

void Box2d::expand(double value)
{
    _min -= value;
    _max += value;
}

void Box2d::moveTo(Point2d const& newCenter)
{
    Point2d ptCurCenter{(_max.x() * 0.5 + _min.x() * 0.5), (_max.y() * 0.5 + _min.y() * 0.5)};
    Point2d offset(newCenter.x() - ptCurCenter.x(), newCenter.y() - ptCurCenter.y());

    _min += offset;
    _max += offset;
}

Box2d Box2d::intersection(Box2d const& box) const
{
    double minX = std::max(_min.x(), box._min.x());
    double maxX = std::min(_max.x(), box._max.x());
    if (minX > maxX)
        return Box2d();

    double minY = std::max(_min.y(), box._min.y());
    double maxY = std::min(_max.y(), box._max.y());
    if (minY > maxY)
        return Box2d();

    return Box2d{Point2d(minX, minY), Point2d(maxX, maxY)};
}

} // namespace sindy
