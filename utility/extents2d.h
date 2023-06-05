#ifndef EXTENTS2D_H
#define EXTENTS2D_H
#include <initializer_list>

#include "qpoint.h"

// 用一个最小的平行于坐标轴的矩形来框住几何体：Minimum Bounding Box
class Extents2d
{
    QPointF m_min;
    QPointF m_max;

public:
    Extents2d();
    Extents2d(const QPointF& pt);
    explicit Extents2d(const std::initializer_list<QPointF>& list);
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
    void addExtents(const Extents2d& ext);
    void operator+=(const Extents2d& ext);
    bool inExtents(const QPointF& pt) const;
    bool inExtents(const QPointF& pt, double tol) const;
    bool outExtents(const QPointF& pt) const;
    bool outExtents(const QPointF& pt, double tol) const;
    bool outExtents(const Extents2d& ext) const;
    bool outExtents(const Extents2d& ext, double tol) const;
};

#endif // !EXTENTS2D_H