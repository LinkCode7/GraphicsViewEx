#ifndef EXTENTS2D_H
#define EXTENTS2D_H
#include <initializer_list>

#include "qpoint.h"

// 用一个最小的平行于坐标轴的矩形来框住几何体：Minimum Bounding Box
class Box2d
{
    QPointF m_min;
    QPointF m_max;

public:
    Box2d();
    Box2d(const QPointF& pt);
    explicit Box2d(const std::initializer_list<QPointF>& list);
    void           set(const QPointF& ptMin, const QPointF& ptMax);
    inline QPointF min() const { return m_min; }
    inline QPointF max() const { return m_max; }

    void    reset();
    void    reset(const QPointF& pt1, const QPointF& pt2);
    bool    invalid() const;
    QPointF centerPt() const;
    void    expand(double value); // 扩大或缩小(负数)包络
    void    moveTo(const QPointF& ptNewCenter);

    void addPoint(const QPointF& pt);
    void add(const Box2d& ext);
    void operator+=(const Box2d& ext);
    bool inBox(const QPointF& pt) const;
    bool inBox(const QPointF& pt, double tol) const;
    bool outBox(const QPointF& pt) const;
    bool outBox(const QPointF& pt, double tol) const;
    bool outBox(const Box2d& ext) const;
    bool outBox(const Box2d& ext, double tol) const;
};

#endif // !EXTENTS2D_H