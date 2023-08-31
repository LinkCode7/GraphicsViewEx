#include "IGePointSet.h"

#include "../utility/SindyMath.h"
#include "../utility/box2d.h"
#include "../utility/utility.h"

IGePointSet::IGePointSet(IGeGraphic::ObjectType type) : IGeGraphic(type)
{
}

IGePointSet::IGePointSet(std::initializer_list<QPointF> const& pts, ObjectType type) : IGeGraphic(type)
{
    for (auto const& pt : pts)
        _points.emplace_back(pt);
}

QRectF IGePointSet::boundingRect() const
{
    Box2d ext;
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

void IGePointSet::list(std::vector<std::pair<std::string, std::string>>& fields) const
{
    IGeGraphic::list(fields);

    std::string str;
    auto        size = _points.size();

    fields.emplace_back(std::make_pair("PointSize", std::to_string(size)));
    for (auto i = 0; i < size; ++i)
    {
        std::string key("points[");
        key += std::to_string(i) + "]";

        str = sindy::simplifyFloat(_points[i].x(), 3) + ", " + sindy::simplifyFloat(_points[i].y(), 3);
        fields.emplace_back(std::make_pair(key, str));

        if (i >= 16)
            return;
    }
}
