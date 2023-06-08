#include "IGePointSet.h"

#include "../utility/extents2d.h"
#include "../utility/utility.h"

IGePointSet::IGePointSet(IGeGraphic::ObjectType type) : IGeGraphic(type)
{
}

IGePointSet::IGePointSet(std::initializer_list<QPointF> const& list, ObjectType type) : IGeGraphic(type)
{
    for (auto const& pt : list)
        _points.emplace_back(pt);
}

QRectF IGePointSet::boundingRect() const
{
    Extents2d ext;
    for (auto const& pt : _points)
        ext.addPoint(pt);

    QRectF rect;
    sindy::extent2Rect(ext.min(), ext.max(), rect);
    return rect;
}

QPainterPath IGePointSet::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}