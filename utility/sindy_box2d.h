#ifndef SINDY_BOX2D_H
#define SINDY_BOX2D_H
#include <initializer_list>

#include "sindy_point2d.h"

namespace sindy
{
// 用一个最小的平行于坐标轴的矩形来框住几何体：Minimum Bounding Box
class Box2d
{
    Point2d _min;
    Point2d _max;

public:
    Box2d();
    Box2d(Point2d const& pt);
    explicit Box2d(const std::initializer_list<Point2d>& list);

    bool           invalid() const;
    inline Point2d min() const { return _min; }
    inline Point2d max() const { return _max; }

    void reset();
    void reset(Point2d const& pt1, Point2d const& pt2);
    void set(Point2d const& ptMin, Point2d const& ptMax);

    void add(Point2d const& pt);
    void add(Box2d const& ext);
    void operator+=(Box2d const& ext);
    bool inBox(Point2d const& pt) const;
    bool inBox(Point2d const& pt, double tol) const;
    bool outBox(Point2d const& pt) const;
    bool outBox(Point2d const& pt, double tol) const;
    bool outBox(Box2d const& ext) const;
    bool outBox(Box2d const& ext, double tol) const;

    // 求交集包围盒(AABB)
    Box2d intersection(Box2d const& box) const;

    void           expand(double value); // 扩大或缩小(负数)包络
    void           moveTo(Point2d const& newCenter);
    inline Point2d center() const { return {(_max.x() * 0.5 + _min.x() * 0.5), (_max.y() * 0.5 + _min.y() * 0.5)}; }

    inline double area() const { return width() - height(); }
    inline double width() const { return _max.x() - _min.x(); }
    inline double height() const { return _max.y() - _min.y(); }

    inline Point2d leftUp() const { return Point2d(_min.x(), _max.y()); }    // 左上
    inline Point2d leftDown() const { return Point2d(_min.x(), _min.y()); }  // 左下
    inline Point2d rightUp() const { return Point2d(_max.x(), _max.y()); }   // 右上
    inline Point2d rightDown() const { return Point2d(_max.x(), _min.y()); } // 右下
};

} // namespace sindy

#endif // !SINDY_BOX2D_H