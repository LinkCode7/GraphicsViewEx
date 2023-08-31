#include "GePolygon.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <iostream>

#include "../utility/JsonData.h"
#include "../utility/SindyMath.h"
#include "../utility/box2d.h"
#include "../utility/utility.h"

namespace sindy
{
void PolySegment::jsonObject(std::vector<std::string>& arr) const
{
    std::string str;
    iguana::to_json(*this, str);
    arr.emplace_back(str);
}
void PolySegment::polyList(std::string const& prefix, std::vector<std::pair<std::string, std::string>>& fields) const
{
    fields.emplace_back(std::make_pair(prefix + "type", std::to_string(int(type))));
    std::string str = sindy::simplifyFloat(begin.x, 3) + ", " + sindy::simplifyFloat(begin.y, 3);
    fields.emplace_back(std::make_pair(prefix + "begin", str));
    str = sindy::simplifyFloat(end.x, 3) + ", " + sindy::simplifyFloat(end.y, 3);
    fields.emplace_back(std::make_pair(prefix + "end", str));
}

void PolyArc::jsonObject(std::vector<std::string>& arr) const
{
    std::string str;
    iguana::to_json(*this, str);
    arr.emplace_back(str);
}
void PolyArc::polyList(std::string const& prefix, std::vector<std::pair<std::string, std::string>>& fields) const
{
    fields.emplace_back(std::make_pair(prefix + "type", std::to_string(int(type))));
    std::string str = sindy::simplifyFloat(center.x, 3) + ", " + sindy::simplifyFloat(center.y, 3);
    fields.emplace_back(std::make_pair(prefix + "center", str));
    fields.emplace_back(std::make_pair(prefix + "radius", sindy::simplifyFloat(radius, 3)));
    fields.emplace_back(std::make_pair(prefix + "radius2", sindy::simplifyFloat(radius2, 3)));
    fields.emplace_back(std::make_pair(prefix + "beginAngle", sindy::simplifyFloat(sindy::radian2Degree(beginAngle), 3)));
    fields.emplace_back(std::make_pair(prefix + "sweepAngle", sindy::simplifyFloat(sindy::radian2Degree(sweepAngle), 3)));
}

std::string Polygon::json() const
{
    JsonString value;
    auto&      arr = value.arrayRef();

    std::string str;
    iguana::to_json(*this, str);
    arr.emplace_back(str);

    for (auto const& element : _elements)
    {
        element->jsonObject(arr);
    }
    return value.make();
}
} // namespace sindy

GePolygon::GePolygon() : IGeGraphic(eGePolygonType)
{
    _color = {0, 255, 0};
}

GePolygon::GePolygon(std::vector<int> const& index) : IGeGraphic(eGePolygonType), _indexes(index)
{
    _color = {0, 255, 0};
}

QRectF GePolygon::boundingRect() const
{
    Box2d box;
    for (auto const& element : _elements)
    {
        if (auto pLine = dynamic_cast<sindy::PolySegment*>(element.get()); pLine)
        {
            box.addPoint({pLine->begin.x, pLine->begin.y});
            box.addPoint({pLine->end.x, pLine->end.y});
        }
        else if (auto pArc = dynamic_cast<sindy::PolyArc*>(element.get()); pArc)
        {
            box.addPoint({pArc->center.x, pArc->center.y});
            box.expand(30.0);
        }
    }

    QRectF rect;
    sindy::extent2Rect(box.min().x(), box.min().y(), box.max().x(), box.max().y(), rect);
    return rect;
}

QPainterPath GePolygon::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void GePolygon::drawArcSegment(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    for (auto const& element : _elements)
    {
        if (auto pLine = dynamic_cast<sindy::PolySegment*>(element.get()); pLine)
        {
            painter->drawLine(pLine->begin.x, pLine->begin.y, pLine->end.x, pLine->end.y);
        }
        else if (auto pArc = dynamic_cast<sindy::PolyArc*>(element.get()); pArc)
        {
            // 第3、4个参数表示圆/椭圆的宽度和高度；
            // 第5、6个参数表示起始角度和扫描角度，[0, 360]，负数表示逆时针旋转，正数表示顺时针旋转
            // Qt 中的角度单位是以 1/16 度为单位的，因此在设置起始角度和弧度时需要将角度值乘以 16
            painter->drawArc(pArc->center.x - pArc->radius, pArc->center.y - pArc->radius2, pArc->radius * 2,
                             pArc->radius2 * 2, sindy::radian2Degree(pArc->beginAngle) * 16,
                             sindy::radian2Degree(pArc->sweepAngle) * 16);
        }
    }
}

void GePolygon::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(QPen(getDrawColor(option->state), getDrawWidth(option->state)));

    if (hasSaveFlag(eNonSegmentEdge))
        return drawArcSegment(painter, option, widget);

    // 连续线段
    for (auto const& element : _elements)
    {
        if (auto pLine = dynamic_cast<sindy::PolySegment*>(element.get()); pLine)
            continue;
        addSaveFlag(eNonSegmentEdge);
        drawArcSegment(painter, option, widget);
        return;
    }

    auto size = _elements.size();
    if (size < 1)
        return; // error

    QVector<QPointF> points;

    auto pLine = static_cast<sindy::PolySegment*>(_elements[0].get());
    points.push_back({pLine->begin.x, pLine->begin.y});

    for (auto i = 0; i < size; ++i)
    {
        auto pLine = static_cast<sindy::PolySegment*>(_elements[i].get());
        points.push_back({pLine->end.x, pLine->end.y});
    }

    painter->drawPolyline(points);
}

bool GePolygon::isLinestring() const
{
    for (auto const& element : _elements)
    {
        auto pLine = dynamic_cast<sindy::PolySegment*>(element.get());
        if (!pLine)
            return false;
    }
    return true;
}

void GePolygon::list(std::vector<std::pair<std::string, std::string>>& fields) const
{
    IGeGraphic::list(fields);

    std::stringstream ss;

    auto size = _indexes.size();
    for (auto i = 0; i < size; ++i)
    {
        ss << std::to_string(_indexes[i]);
        if (i < size - 1)
            ss << ", ";
    }
    fields.emplace_back(std::make_pair("indexes", ss.str()));

    size = _elements.size();

    fields.emplace_back(std::make_pair("EdgeSize", std::to_string(size)));
    for (auto i = 0; i < size; ++i)
    {
        std::string prefix("[");
        prefix += std::to_string(i) + "].";
        _elements[i]->polyList(prefix, fields);

        if (i >= 16)
            return;
    }
}