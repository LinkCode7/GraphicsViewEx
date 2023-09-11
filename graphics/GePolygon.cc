#include "GePolygon.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <iostream>

#include "../utility/JsonData.h"
#include "../utility/SindyMath.h"
#include "../utility/sindy_box2d.h"
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
    fields.emplace_back(std::make_pair(prefix + "type", std::to_string(int(_type))));
    std::string str = sindy::simplifyFloat(_begin.x, 3) + ", " + sindy::simplifyFloat(_begin.y, 3);
    fields.emplace_back(std::make_pair(prefix + "begin", str));
    str = sindy::simplifyFloat(_end.x, 3) + ", " + sindy::simplifyFloat(_end.y, 3);
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
    fields.emplace_back(std::make_pair(prefix + "type", std::to_string(int(_type))));
    std::string str = sindy::simplifyFloat(_center.x, 3) + ", " + sindy::simplifyFloat(_center.y, 3);
    fields.emplace_back(std::make_pair(prefix + "center", str));
    fields.emplace_back(std::make_pair(prefix + "radius", sindy::simplifyFloat(_radius, 3)));
    fields.emplace_back(std::make_pair(prefix + "radius2", sindy::simplifyFloat(_radius2, 3)));
    fields.emplace_back(std::make_pair(prefix + "beginAngle", sindy::simplifyFloat(sindy::radian2Degree(_beginAngle), 3)));
    fields.emplace_back(std::make_pair(prefix + "sweepAngle", sindy::simplifyFloat(sindy::radian2Degree(_sweepAngle), 3)));
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
    sindy::Box2d box;
    for (auto const& element : _elements)
    {
        if (auto pLine = dynamic_cast<sindy::PolySegment*>(element.get()); pLine)
        {
            box.add({pLine->_begin.x, pLine->_begin.y});
            box.add({pLine->_end.x, pLine->_end.y});
        }
        else if (auto pArc = dynamic_cast<sindy::PolyArc*>(element.get()); pArc)
        {
            box.add({pArc->_center.x, pArc->_center.y});
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
            painter->drawLine(pLine->_begin.x, pLine->_begin.y, pLine->_end.x, pLine->_end.y);
        }
        else if (auto pArc = dynamic_cast<sindy::PolyArc*>(element.get()); pArc)
        {
            // 第3、4个参数表示圆/椭圆的宽度和高度；
            // 第5、6个参数表示起始角度和扫描角度，[0, 360]，负数表示逆时针旋转，正数表示顺时针旋转
            // Qt 中的角度单位是以 1/16 度为单位的，因此在设置起始角度和弧度时需要将角度值乘以 16
            painter->drawArc(pArc->_center.x - pArc->_radius, pArc->_center.y - pArc->_radius2, pArc->_radius * 2,
                             pArc->_radius2 * 2, sindy::radian2Degree(pArc->_beginAngle) * 16,
                             sindy::radian2Degree(pArc->_sweepAngle) * 16);
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
    points.push_back({pLine->_begin.x, pLine->_begin.y});

    for (auto i = 0; i < size; ++i)
    {
        auto pLine = static_cast<sindy::PolySegment*>(_elements[i].get());
        points.push_back({pLine->_end.x, pLine->_end.y});
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

bool GePolygon::getPoints(std::vector<std::vector<sindy::Point>>& output) const
{
    auto size = _elements.size();
    if (size == 0)
        return false;

    auto mat = this->sceneMatrix();

    auto count = _indexes.size();
    if (count == 0 || count == 1)
    {
        std::vector<sindy::Point> pts;
        pts.emplace_back(_elements[0]->begin().translate(mat.dx(), mat.dy()));
        for (auto i = 1; i < size; ++i)
        {
            auto pt = _elements[i]->begin().translate(mat.dx(), mat.dy());
            pts.emplace_back(pt);
        }
        pts.emplace_back(_elements[size - 1]->end().translate(mat.dx(), mat.dy()));

        makePolyClockwise(pts);
        output.emplace_back(pts);
        return true;
    }

    // 外轮廓 + 内洞
    size_t                    index = 0;
    std::vector<sindy::Point> pts;
    for (auto i = 0; i < count; ++i)
    {
        size_t num = _indexes[i];

        pts.clear();
        pts.emplace_back(_elements[index]->begin().translate(mat.dx(), mat.dy()));
        for (auto j = index + 1; j < num; ++j)
            pts.emplace_back(_elements[j]->begin().translate(mat.dx(), mat.dy()));
        pts.emplace_back(_elements[num - 1]->end().translate(mat.dx(), mat.dy()));

        index += num;

        makePolyClockwise(pts);
        output.emplace_back(pts);
    }
    return true;
}

bool GePolygon::isPolyClockwise(std::vector<sindy::Point> const& pts)
{
    auto size = pts.size();
    if (size < 3)
        return true; // error

    double sum = 0.0;
    for (auto i = 1; i < size; ++i)
    {
        auto p  = pts[i - 1];
        auto p1 = pts[i];
        sum += (p1.x + p.x) * (p1.y, p.y);
    }

    auto p0 = pts[0];
    auto pn = pts[size - 1];
    sum += (p0.x + pn.x) * (p0.y - pn.y);
    return sum <= 0;
}

void GePolygon::makePolyClockwise(std::vector<sindy::Point>& pts)
{
    if (isPolyClockwise(pts))
        return;

    std::reverse(pts.begin(), pts.end());
}

std::string GePolygon::booleanIntersectionName(GePolygon* poly1, GePolygon* poly2)
{
    if (!poly1 || !poly2)
        return {};

    std::string name("boolean:");
    return name + std::to_string(poly1->id()) + BOOLEAN_INTERSECTION_SYMBOL + std::to_string(poly2->id());
}

std::string GePolygon::booleanUnionName(std::vector<GePolygon*> const& polys)
{
    std::string name("boolean:");

    auto size = polys.size();
    if (size == 0)
        return name;

    name = std::to_string(polys[0]->id());
    for (auto i = 1; i < size; ++i)
    {
        name += BOOLEAN_UNION_SYMBOL + std::to_string(polys[i]->id());
    }
    return name;
}
std::string GePolygon::booleanDifferenceName(std::vector<GePolygon*> const& polys)
{
    std::string name("boolean:");

    auto size = polys.size();
    if (size == 0)
        return name;

    name = std::to_string(polys[0]->id());
    for (auto i = 1; i < size; ++i)
    {
        name += BOOLEAN_DIFFERENCE_SYMBOL + std::to_string(polys[i]->id());
    }
    return name;
}