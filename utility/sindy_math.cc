#include "sindy_math.h"

#include "sindy_box2d.h"

double sindy::roundFloat(double value, uint8_t num)
{
    if (num == 0)
        return value;

    double srcZS    = floor(value);
    double lngMulti = pow((double)10, num);

    value -= srcZS;
    value *= lngMulti;
    double src = floor(value);
    if (value - src >= 0.5)
    {
        double dblXS = (src + 1) / lngMulti;
        return srcZS + dblXS;
    }

    double dblXS = src / lngMulti;
    return srcZS + dblXS;
}

std::string sindy::simplifyFloat(double value, uint8_t num)
{
    if (value < 0.0)
    {
        auto result = simplifyFloat(-value, num);
        if (result != "0")
            result = '-' + result;
        return result;
    }

    // 分离整数、小数
    uint64_t iFloor = floor(value);
    double   dec    = value - iFloor;

    uint64_t pow = powTen(num);

    // 四舍五入
    double decNew = dec * pow;

    // 通过float精度转换实现四舍五入
    uint64_t floorNew = (double)(decNew + 0.5);

    // 可能出现进位至整数部分
    if (floorNew == pow)
    {
        floorNew = 0;
        ++iFloor;
    }

    // 整数部分
    std::string strFloor = std::to_string(iFloor);
    if (floorNew <= 0)
        return strFloor;

    std::string& ret = strFloor;
    // 小数部分，需要处理前置位补0和后置位去0操作。如保留三位小数时，小数部分是60，但显示为0.06
    std::string strDec = std::to_string(floorNew);

    int lenDec = strDec.size();
    if (lenDec == num)
    {
        ret.push_back('.');
        ret += strDec;
        trimInvalid0(ret);
    }
    else
    {
        ret.push_back('.');
        lenDec = num - lenDec;
        while (--lenDec >= 0)
        {
            ret.push_back('0');
        }
        trimInvalid0(strDec);
        ret += strDec;
    }

    return ret;
}

void sindy::trimInvalid0(std::string& strFloat)
{
    int end = strFloat.size();
    for (int i = end - 1; i >= 0; --i)
    {
        if (strFloat[i] != '0')
            break;
        --end;
    }
    if (end == strFloat.size())
        return;

    strFloat = std::string(strFloat.begin(), strFloat.begin() + end);
}

uint64_t sindy::powTen(uint8_t num)
{
    uint64_t multiplier = 1;
    switch (num)
    {
        case 0:
            break;
        case 1:
            multiplier = 10;
            break;
        case 2:
            multiplier = 100;
            break;
        case 3:
            multiplier = 1000;
            break;
        case 4:
            multiplier = 10000;
            break;
        case 5:
            multiplier = 100000;
            break;
        case 6:
            multiplier = 1000000;
            break;
        case 7:
            multiplier = 10000000;
            break;
        case 8:
            multiplier = 100000000;
            break;
        case 9:
            multiplier = 1000000000;
            break;
        case 10:
            multiplier = 10000000000;
            break;
        default:
            for (auto i = 0; i < num; ++i)
            {
                multiplier *= 10;
            }
            break;
    }
    return multiplier;
}

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
    if (Box2d{a, b}.outBox(Box2d{c, d}))
        return false;

    double s1 = Point2d::crossProduct(a, b, c);
    double s2 = Point2d::crossProduct(a, b, d);
    double s3 = Point2d::crossProduct(c, d, a);
    double s4 = Point2d::crossProduct(c, d, b);
    return (s1 * s2 < 0) && (s3 * s4 < 0);
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

    double cross_product = 0;
    for (int i = 0; i < size; i++)
    {
        auto& p1 = pts[i];
        auto& p2 = pts[(i + 1) % size];
        cross_product += (p2.y() - p1.y()) * (p2.x() + p1.x());
    }
    return cross_product >= 0;
}
