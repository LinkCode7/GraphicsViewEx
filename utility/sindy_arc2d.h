#ifndef SINDY_ARC2D_H
#define SINDY_ARC2D_H

#include <cmath>

#include "sindy_geometry.h"
#include "sindy_point2d.h"

// 时钟方向
enum class ClockDirection
{
    eUnknownClockwise,
    eClockwise,        // 顺时针
    eCounterClockwise, // 逆时针
};

namespace sindy
{
/*
 * @brief 圆弧、椭圆
 * @detail 角度均采用弧度制，Point2d::kXAxis为0°位置，值大于0为逆时针，范围[-2PI, 2PI]
 */
class Arc2d : public Geometry
{
    Point2d _begin; // 起点
    Point2d _end;   // 终点

    Point2d _center;           // 圆心
    double  _radius     = 0.0; // 半径w
    double  _radius2    = 0.0; // 半径h
    double  _beginAngle = 0.0; // 起点角度
    double  _sweepAngle = 0.0; // 扫掠角度

public:
    Arc2d(Arc2d const&)            = delete;
    Arc2d& operator=(Arc2d const&) = delete;
    Arc2d(Point2d const& begin, Point2d const& end, double angle);
    Arc2d(Point2d const& center, double beginAngle, double endAngle, ClockDirection dir, double radius, double radius2);
    Arc2d(Point2d const& center, Point2d const& begin, Point2d const& end, ClockDirection dir, double radius,
          double radius2);

public:
    Point2d begin() const override { return _begin; }
    void    begin(Point2d const& value) { _begin = value; }

    Point2d end() const { return _end; }
    void    end(Point2d const& value) { _end = value; }

    Point2d center() const { return _center; }
    void    center(Point2d const& value) { _center = value; }

    double radius() const { return _radius; }
    void   radius(double value) { _radius = value; }

    double beginAngle() const { return _beginAngle; }
    void   beginAngle(double value) { _beginAngle = value; }

    double endAngle() const { return _beginAngle + _sweepAngle; }

    double sweepAngle() const { return _sweepAngle; }
    void   sweepAngle(double value) { _sweepAngle = value; }

    ClockDirection direction() const
    {
        return _sweepAngle >= 0.0 ? ClockDirection::eCounterClockwise : ClockDirection::eClockwise;
    }
    bool isCCW() const { return _sweepAngle >= 0.0; }

public:
    double length() const override { return (2 * PI * _radius) * (fabs(_sweepAngle) / RADIAN_360); }

    std::vector<Point2d> segment(double unitLength) const override;

    static inline double normalize(double radian)
    {
        radian = std::fmod(radian, PI2);
        if (radian < 0.0)
            radian = PI2 + radian;
        return radian == PI2 ? 0.0 : radian;
    }

    // 求圆弧上的点
    Point2d getPoint(double radian) const;

    // 根据起点、终点、夹角求圆心
    static bool getCenterPoint(Point2d const& begin, Point2d const& end, double radian, Point2d& center);

public:
    int intersection(Arc2d const& arc, std::vector<Point2d>& result) const;

private:
    void calculateAngle(double beginAngle, double endAngle, ClockDirection dir);
};
} // namespace sindy

#endif // !SINDY_ARC2D_H
