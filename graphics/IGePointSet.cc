#include "IGePointSet.h"

#include "../utility/sindy_box2d.h"
#include "../utility/sindy_math.h"
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
    sindy::Box2d ext;
    for (auto const& pt : _points)
        ext.add(sindy::Point2d{pt.x(), pt.y()});

    QRectF rect;
    sindy::extent2Rect(QPoint(ext.min().x(), ext.min().y()), QPoint(ext.max().x(), ext.max().y()), rect);
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
