#include "BoostBoolean.h"

#include "../graphics/GePolygon.h"

#ifdef USING_BOOST_BOOLEAN

    #include <boost/geometry.hpp>
    #include <boost/geometry/algorithms/intersection.hpp>
    #include <boost/geometry/geometries/adapted/c_array.hpp>
    #include <boost/geometry/geometries/point.hpp>
    #include <boost/geometry/geometries/point_xy.hpp>
    #include <boost/geometry/geometries/polygon.hpp>
    #include <boost/geometry/geometries/segment.hpp>
    #include <iostream>
    #include <memory>
    #include <unordered_set>
    #include <vector>

namespace bg = boost::geometry;

// BOOST_GEOMETRY_REGISTER_POINT_2D(sindy::Point, double, bg::cs::cartesian, x, y)

// typedef bg::model::point<double, 2, bg::cs::cartesian> point_2d;
typedef bg::model::d2::point_xy<double> point_2d;
typedef bg::model::segment<point_2d>    segment_2d;
typedef bg::model::polygon<point_2d>    polygon_2d;

#endif

void boolean::intersection(GePolygon* poly1, GePolygon* poly2, std::vector<GePolygon*>& result)
{
    std::vector<std::vector<sindy::Point>> pts1;
    if (!poly1->getPoints(pts1))
        return;

    std::vector<std::vector<sindy::Point>> pts2;
    if (!poly2->getPoints(pts2))
        return;

#ifdef USING_BOOST_BOOLEAN
    // y轴颠倒...false:GraphicsView的Y轴和笛卡尔坐标系相反
    typedef boost::geometry::model::d2::point_xy<double>       point_type;
    typedef boost::geometry::model::polygon<point_type, false> polygon;

    auto assign = [&](std::vector<std::vector<sindy::Point>> const& pts, polygon& poly) {
        auto size = pts.size();
        if (size == 0)
            return;

        for (auto const& pt : pts[0])
            bg::append(poly.outer(), point_type(pt.x(), pt.y()));

        if (size == 1)
            return;

        poly.inners().resize(size - 1);
        for (auto i = 1; i < size; ++i)
        {
            for (auto const& pt : pts[i])
                bg::append(poly.inners()[i - 1], point_type(pt.x(), pt.y()));
        }
    };

    polygon p1, p2;
    assign(pts1, p1);
    assign(pts2, p2);

    std::deque<polygon> output;
    boost::geometry::intersection(p1, p2, output);

    // y轴颠倒...
    auto name = GePolygon::booleanIntersectionName(poly1, poly2);
    for (auto const& item : output)
    {
        auto pNew = new GePolygon();
        pNew->name(name);

        auto len = item.outer().size();
        for (auto i = 1; i < len; ++i)
        {
            auto ptBegin = item.outer()[i - 1];
            auto ptEnd   = item.outer()[i];
            auto seg     = std::make_shared<sindy::PolySegment>(sindy::Point{ptBegin.x(), ptBegin.y()},
                                                            sindy::Point{ptEnd.x(), ptEnd.y()});
            pNew->addNewEdge(seg);
        }
        pNew->indexesRef().emplace_back(len);

        auto length = item.inners().size();
        for (auto i = 0; i < length; ++i)
        {
            auto const& edges = item.inners()[i];

            len = edges.size();
            if (len == 0)
                continue;

            for (auto i = 1; i < len; ++i)
            {
                auto ptBegin = edges[i - 1];
                auto ptEnd   = edges[i];
                auto seg     = std::make_shared<sindy::PolySegment>(sindy::Point{ptBegin.x(), ptBegin.y()},
                                                                sindy::Point{ptEnd.x(), ptEnd.y()});
                pNew->addNewEdge(seg);
            }

            pNew->indexesRef().emplace_back(len);
        }

        result.emplace_back(pNew);
    }

#endif
}
