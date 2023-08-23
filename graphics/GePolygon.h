#ifndef GE_POLYGON_H
#define GE_POLYGON_H

#include "../iguana/iguana/json_writer.hpp"
#include "IGeGraphic.h"

#ifndef PI
    #define PI 3.14159265358979323846    // pi
    #define PI2 6.28318530717958623200   // pi*2
    #define PI_2 1.57079632679489661923  // pi/2
    #define PI_4 0.785398163397448309616 // pi/4
#endif

namespace sindy
{
struct Point
{
    double x = 0.0;
    double y = 0.0;
    Point() {}
    Point(double xx, double yy) : x(xx), y(yy) {}
};
REFLECTION(Point, x, y);

struct Geometry
{
    virtual ~Geometry() {}
    virtual void jsonObject(std::vector<std::string> &arr) const = 0;
};
using GeometrySp = std::shared_ptr<Geometry>;

struct Segment : public Geometry
{
public:
    IGeGraphic::ObjectType type = IGeGraphic::ObjectType::eGeSegmentType;
    Point                  begin;
    Point                  end;
    Segment() {}
    Segment(Point const &begin1, Point const &end1) : begin(begin1), end(end1) {}

    void jsonObject(std::vector<std::string> &arr) const override;
};
REFLECTION(Segment, type, begin, end);

struct Arc : public Geometry
{
    IGeGraphic::ObjectType type = IGeGraphic::ObjectType::eGeArcType;
    Point                  center;           // 中心点
    double                 radius     = 0.0; // 半径
    double                 beginAngle = 0.0;
    double                 sweepAngle = 0.0;
    Arc() {}
    Arc(Point const &center1, double radius1, double beginAngle1, double sweepAngle1)
        : center(center1), radius(radius1), beginAngle(beginAngle1), sweepAngle(sweepAngle1)
    {
    }

    void jsonObject(std::vector<std::string> &arr) const override;
};
REFLECTION(Arc, type, center, radius, beginAngle, sweepAngle);

struct Polygon
{
    IGeGraphic::ObjectType type = IGeGraphic::ObjectType::eGePolygonType;
    std::vector<int>       index; // 轮廓边或内洞边的数量，至少存在一个元素

    std::vector<GeometrySp> _elements; // 保存基类指针

    Polygon(std::vector<int> const &index1) : index(index1) {}

    std::string json() const;
};
REFLECTION(Polygon, type, index); // 边要自己添加

using PolygonSp = std::shared_ptr<Polygon>;
} // namespace sindy

/*
 * @brief 多边形
 */
class GePolygon : public IGeGraphic
{
    std::vector<int>               _index;    // 轮廓边或内洞边的数量，至少存在一个元素
    std::vector<sindy::GeometrySp> _elements; // 保存基类指针

    VISIT_THIS_CLASS
public:
    GePolygon();
    GePolygon(std::vector<int> const &index);

    void addEdge(sindy::GeometrySp element) { _elements.emplace_back(element); }

    QPainterPath shape() const override;
    QRectF       boundingRect() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // 对象创建相关的业务
    std::unique_ptr<GraphicMaker> subMake() const override { return std::make_unique<PolygonMaker>(); }

public:
    static inline constexpr double radian2Degree(double radian) { return (radian * 180) / PI; }
    static inline constexpr double degree2Radian(double degree) { return (degree / 180) * PI; }
};

#endif // !GE_POLYGON_H
