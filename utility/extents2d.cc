#include "extents2d.h"

#include "float.h"

#define SINDY_ZERO 0.00001 // 1e-5
#define MY_EXTENTS_MAX -DBL_MAX
#define MY_EXTENTS_MIN DBL_MAX

bool compareDouble(double value1, double value2, double tol = SINDY_ZERO)
{
    double sub = value1 - value2;
    if (sub < 0)
        sub = -sub;

    if (sub <= tol)
        return true;
    return false;
}

Extents2d::Extents2d() : m_min{MY_EXTENTS_MIN, MY_EXTENTS_MIN}, m_max{MY_EXTENTS_MAX, MY_EXTENTS_MAX}
{
}
Extents2d::Extents2d(const QPointF& pt) : m_min(pt), m_max(pt)
{
}
Extents2d::Extents2d(const std::initializer_list<QPointF>& list)
    : m_min{MY_EXTENTS_MIN, MY_EXTENTS_MIN}, m_max{MY_EXTENTS_MAX, MY_EXTENTS_MAX}
{
    for (const auto& point : list)
        addPoint(point);
}

void Extents2d::reset()
{
    m_min = {MY_EXTENTS_MIN, MY_EXTENTS_MIN};
    m_max = {MY_EXTENTS_MAX, MY_EXTENTS_MAX};
}
void Extents2d::reset(const QPointF& pt1, const QPointF& pt2)
{
    m_min = pt1;
    m_max = pt1;
    addPoint(pt2);
}
void Extents2d::set(const QPointF& ptMin, const QPointF& ptMax)
{
    m_min = ptMin;
    m_max = ptMax;
}

// 建议使用前先判断有效性
bool Extents2d::invalid() const
{
    if (compareDouble(m_min.x(), MY_EXTENTS_MIN) && compareDouble(m_min.y(), MY_EXTENTS_MIN) && compareDouble(m_max.x(), MY_EXTENTS_MAX) &&
        compareDouble(m_max.y(), MY_EXTENTS_MAX))
        return true;
    return false;
}
void Extents2d::addPoint(const QPointF& pt)
{
    if (pt.x() < m_min.x())
        m_min.setX(pt.x());
    if (pt.x() > m_max.x())
        m_max.setX(pt.x());

    if (pt.y() < m_min.y())
        m_min.setY(pt.y());
    if (pt.y() > m_max.y())
        m_max.setY(pt.y());
}
void Extents2d::addExtents(const Extents2d& ext)
{
    addPoint(ext.m_min);
    addPoint(ext.m_max);
}
void Extents2d::operator+=(const Extents2d& ext)
{
    addPoint(ext.m_min);
    addPoint(ext.m_max);
}

bool Extents2d::inExtents(const QPointF& pt) const
{
    if (pt.x() < m_min.x() || pt.x() > m_max.x())
        return false;
    if (pt.y() < m_min.y() || pt.y() > m_max.y())
        return false;
    return true;
}
bool Extents2d::inExtents(const QPointF& pt, double tol) const
{
    if (pt.x() < m_min.x() - tol || pt.x() > m_max.x() + tol)
        return false;
    if (pt.y() < m_min.y() - tol || pt.y() > m_max.y() + tol)
        return false;
    return true;
}
bool Extents2d::outExtents(const QPointF& pt) const
{
    if (pt.x() < m_min.x() || pt.x() > m_max.x())
        return true;
    if (pt.y() < m_min.y() || pt.y() > m_max.y())
        return true;
    return false;
}
bool Extents2d::outExtents(const QPointF& pt, double tol) const
{
    if (pt.x() < m_min.x() - tol || pt.x() > m_max.x() + tol)
        return true;
    if (pt.y() < m_min.y() - tol || pt.y() > m_max.y() + tol)
        return true;
    return false;
}
bool Extents2d::outExtents(const Extents2d& ext) const
{
    if (ext.m_max.x() < m_min.x() || ext.m_min.x() > m_max.x())
        return true;
    if (ext.m_max.y() < m_min.y() || ext.m_min.y() > m_max.y())
        return true;
    return false;
}
bool Extents2d::outExtents(const Extents2d& ext, double tol) const
{
    if (ext.m_max.x() < m_min.x() - tol || ext.m_min.x() > m_max.x() + tol)
        return true;
    if (ext.m_max.y() < m_min.y() - tol || ext.m_min.y() > m_max.y() + tol)
        return true;
    return false;
}

void Extents2d::expand(double value)
{
    m_min -= {value, value};
    m_max += {value, value};
}

void Extents2d::moveTo(const QPointF& ptNewCenter)
{
    QPointF ptCurCenter{(m_max.x() * 0.5 + m_min.x() * 0.5), (m_max.y() * 0.5 + m_min.y() * 0.5)};
    auto    offset = ptNewCenter - ptCurCenter;
    m_min += offset;
    m_max += offset;
}

QPointF Extents2d::centerPt() const
{
    return {(m_max.x() * 0.5 + m_min.x() * 0.5), (m_max.y() * 0.5 + m_min.y() * 0.5)};
}