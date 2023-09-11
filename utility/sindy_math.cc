#include "sindy_math.h"

#include "sindy_box2d.h"

bool sindy::isPtInLine(Point2d const& a, Point2d const& b, Point2d const& c)
{
    Point2d pt1(c - a);
    Point2d pt2(c - b);
    auto    cross = pt1.x() * pt2.y() - pt1.y() * pt2.x(); // 叉积
    // 点c在ab的直线上
    if (compare(cross, 0.0) != 0)
        return false;

    // 点c不在直线ab的延长线上
    Box2d ext({a, b});
    if (ext.outBox(c))
        return false;
    return true;
}

bool sindy::isIntersect(Point2d const& a, Point2d const& b, Point2d const& c, Point2d const& d)
{
    // 1. 碰撞检测
    if (Box2d{a, b}.outBox(Box2d{c, d}))
        return false;

    // 2. 跨立检测
    // 点a、b在线段cd的两侧，则有：
    auto dc = c - d;
    if ((a - d).crossProduct(dc) * (b - d).crossProduct(dc) <= 0.0) // ad、bd位于cd两侧
    {
        // 点c、d在线段ab的两侧，则有：
        auto ba = a - b;
        if ((c - b).crossProduct(ba) * (d - b).crossProduct(ba) <= 0.0) // cb、db位于ab两侧
            return true;
    }

    return false;
}

// 判断两条线段是否相交
// 如果两线段相交，则两线段必然相互跨立对方,通过叉积来做判断即可。但需要先使用快速排斥试验处理可能的共线等问题。
// x1,y1,x2,y2为第一个线段的两个点；x3,y3,x4,y4为第二个线段的两个点。
bool is_segment_cross(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    // 先使用快速排斥试验进行判断，同时排除两条线段共线的情况
    if (std::max(x1, x2) < std::min(x3, x4) || std::max(y1, y2) < std::min(y3, y4) || std::min(x1, x2) > std::max(x3, x4) ||
        std::min(y1, y2) > std::max(y3, y4))
        return false;

    // 使用跨立试验进行相交判断
    if (((x3 - x1) * (y3 - y4) - (y3 - y1) * (x3 - x4)) * ((x3 - x2) * (y3 - y4) - (y3 - y2) * (x3 - x4)) <= 0 &&
        ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) * ((x1 - x4) * (y1 - y2) - (y1 - y4) * (x1 - x2)) <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<sindy::Point2d> sindy::intersection(Point2d const& a, Point2d const& b, Point2d const& c, Point2d const& d)
{
    // 快速判断
    auto box = Box2d({a, b}).intersection(Box2d({c, d}));
    if (box.invalid())
        return {};

    auto c1 = Point2d::crossProduct(c, d, a);
    auto c2 = Point2d::crossProduct(c, d, b);
    if (fabs(c1) < SINDY_PRECISION || fabs(c2) < SINDY_PRECISION || c1 * c2 < 0)
    {
    }
    else
    {
        return {};
    }

    double x1 = a.x(); // line1.begin
    double y1 = a.y();
    double x2 = b.x(); // line1.end
    double y2 = b.y();
    double x3 = c.x(); // line2.begin
    double y3 = c.y();
    double x4 = d.x(); // line2.end
    double y4 = d.y();

    auto cross1 = Point2d::crossProduct(x1, y1, x3, y3, x4, y4);
    auto cross2 = Point2d::crossProduct(x2, y2, x3, y3, x4, y4);
    if (cross1 * cross2 > 0 && compare(cross1, 0.0, SINDY_PRECISION) != 0 && compare(cross2, 0.0, SINDY_PRECISION) != 0)
        return {};

    auto cross3 = Point2d::crossProduct(x3, y3, x1, y1, x2, y2);
    auto cross4 = Point2d::crossProduct(x4, y4, x1, y1, x2, y2);
    if (cross3 * cross4 > 0 && compare(cross3, 0.0, SINDY_PRECISION) != 0 && compare(cross4, 0.0, SINDY_PRECISION) != 0)
        return {};

    std::vector<sindy::Point2d> result;

    int cmp1 = compare(cross1, 0.0, SINDY_PRECISION);
    int cmp2 = compare(cross2, 0.0, SINDY_PRECISION);
    if (cmp1 == 0 && cmp2 == 0) // 重合
    {
        auto width  = box.width();
        auto height = box.height();
        if (width == 0 && height == 0)
        {
            result.emplace_back(box.leftUp());
        }
        else if (width == 0)
        {
            result.emplace_back(box.leftUp());
            result.emplace_back(box.leftDown());
        }
        else if (height == 0)
        {
            result.emplace_back(box.leftUp());
            result.emplace_back(box.rightUp());
        }
        else
        {
            auto areaLeftUp  = triangleArea(box.leftUp(), c, d);
            auto areaRightUp = triangleArea(box.rightUp(), c, d);
            if (areaLeftUp < areaRightUp)
            {
                result.emplace_back(box.leftUp());
                result.emplace_back(box.rightDown());
            }
            else
            {
                result.emplace_back(box.rightUp());
                result.emplace_back(box.leftDown());
            }
        }
    }
    else if (cmp1 == 0) // 端点处相交
    {
        result.emplace_back(a);
    }
    else if (cmp2 == 0) // 端点处相交
    {
        result.emplace_back(b);
    }
    else // 常规情况，非端点相交，非重合
    {
        auto nextRatio = fabs(cross1 / (cross1 - cross2));
        result.emplace_back(Point2d(x1 + (x2 - x1) * nextRatio, y1 + (y2 - y1) * nextRatio));
    }

    return result;
}

bool sindy::inTriangle(Point2d const& a, Point2d const& b, Point2d const& c, Point2d const& pt)
{
    Point2d v0 = c - a;
    Point2d v1 = b - a;
    Point2d v2 = pt - a;

    double dot00 = v0 * v0;
    double dot01 = v0 * v1;
    double dot02 = v0 * v2;
    double dot11 = v1 * v1;
    double dot12 = v1 * v2;

    double inverDeno = 1 / (dot00 * dot01 - dot01 * dot01);
    double u         = (dot11 * dot02 - dot01 * dot12);
    if (u < 0 || u > 1)
        return false;

    double v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
    if (v < 0 || v > 1)
        return false;

    return u + v <= 1;
}

bool sindy::isArcClockwise(Point2d const& begin, Point2d const& end, Point2d const& center)
{
    auto vct1 = begin - center;
    auto vct2 = end - center;
    if (vct1.crossProduct(vct2) > 0.0)
        return false; // 逆时针ccw

    return true; // 顺时针
}

bool sindy::isPolyClockwise(std::vector<sindy::Point2d> const& pts)
{
    auto size = pts.size();
    if (size < 3)
        return true; // error

    double sum = 0.0;
    for (auto i = 1; i < size; ++i)
    {
        auto p  = pts[i - 1];
        auto p1 = pts[i];
        sum += (p1.x() + p.x()) * (p1.y(), p.y());
    }

    auto p0 = pts[0];
    auto pn = pts[size - 1];
    sum += (p0.x() + pn.x()) * (p0.y() - pn.y());
    return sum <= 0;
}
