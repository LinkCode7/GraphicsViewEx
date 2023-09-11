#include "sindy_arc2d.h"

#include "sindy_math.h"

namespace sindy
{
Arc2d::Arc2d(Point2d const& begin, Point2d const& end, double radian)
{
    auto angle = fabs(radian) / 2;
    if (angle == 0)
        return; // error

    Point2d direct = end - begin;
    double  radius = direct.length() / 2 / sin(angle);

    Point2d v       = Point2d(-direct.y(), direct.x()).normalize();
    Point2d direct2 = v.rotate(Point2d(0.0, 0.0), angle);

    ClockDirection sweep = ClockDirection::eClockwise;
    if (radian > 0.0)
    {
        sweep   = ClockDirection::eCounterClockwise;
        _center = end + (direct2.scale(radius));
    }
    else
    {
        _center = begin - (direct2.scale(radius));
    }

    _radius  = radius;
    _radius2 = _radius;
    _begin   = begin;
    _end     = end;
    calculateAngle(begin.angle(_center), end.angle(_center), sweep);
}

Arc2d::Arc2d(Point2d const& center, double beginAngle, double endAngle, ClockDirection dir, double radius, double radius2)
    : _center(center), _radius(radius2)
{
    calculateAngle(beginAngle, endAngle, dir);
    _begin = getPoint(beginAngle);
    _end   = getPoint(endAngle);
}

Arc2d::Arc2d(Point2d const& center, Point2d const& begin, Point2d const& end, ClockDirection dir, double radius,
             double radius2)
    : _center(center), _radius(radius), _radius2(radius2)
{
    calculateAngle(begin.angle(center), end.angle(center), dir);
}

Point2d Arc2d::getPoint(double radian) const
{
    radian = Arc2d::normalize(radian);

    auto const& rx = _radius;
    auto const& ry = _radius2;

    if (radian == RADIAN_0)
        return _center.getAdd(rx, 0);
    if (radian == RADIAN_90)
        return _center.getAdd(0, ry);
    if (radian == RADIAN_180)
        return _center.getAdd(-rx, 0);
    if (radian == RADIAN_270)
        return _center.getAdd(0, -ry);

    double rx2 = rx * rx;
    double ry2 = ry * ry;

    // 椭圆公式
    double tg  = tan(radian);
    double tg2 = tg * tg;
    double x   = sqrt((rx2 * ry2) / (rx2 + ry2 / tg2));
    double y   = sqrt((rx2 * ry2) / (ry2 + rx2 / tg2));

    if (radian > RADIAN_90 && radian < RADIAN_270)
        x *= -1;
    if (radian > RADIAN_180 && radian < RADIAN_360)
        y *= -1;

    if (isnan(x))
        x = 0.0;
    if (isnan(y))
        y = 0.0;

    return _center.getAdd(x, y);
}

bool Arc2d::getCenterPoint(Point2d const& begin, Point2d const& end, double radian, Point2d& center)
{
    auto angle = fabs(radian) / 2;
    if (angle == 0)
        return false;

    Point2d direct = end - begin;
    double  radius = direct.length() / 2 / sin(angle);

    Point2d v       = Point2d(-direct.y(), direct.x()).normalize();
    Point2d direct2 = v.rotate(Point2d(0.0, 0.0), angle);
    if (radian > 0.0)
        center = end + (direct2.scale(radius));
    else
        center = begin - (direct2.scale(radius));
    return true;
}

void Arc2d::calculateAngle(double beginAngle, double endAngle, ClockDirection dir)
{
    beginAngle = normalize(beginAngle);
    endAngle   = normalize(endAngle);

    int cmp = sindy::compare(beginAngle, endAngle);
    if (cmp != 0)
    {
        if (dir == ClockDirection::eCounterClockwise)
        {
            if (cmp == 1)
                endAngle += RADIAN_360;
        }
        else
        {
            if (cmp == -1)
                beginAngle += RADIAN_360;
        }
        _sweepAngle = endAngle - beginAngle;
    }
    else
    {
        _sweepAngle = 0.0;
    }

    _beginAngle = beginAngle;
}

int Arc2d::intersection(Arc2d const& arc, std::vector<Point2d>& result) const
{
    return 0;
}

std::vector<Point2d> Arc2d::segment(double unitLength) const
{
    double len   = length();
    double count = ceil(len / unitLength);

    double angle = _beginAngle;
    double step  = _sweepAngle / count;

    std::vector<Point2d> result;
    result.reserve(count);
    for (auto i = 0; i <= count; ++i)
    {
        result.emplace_back(getPoint(angle));
        angle += step;
    }

    return result;
}

} // namespace sindy
