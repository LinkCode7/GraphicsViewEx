#ifndef GE_POLYGON_H
#define GE_POLYGON_H

#include "../iguana/iguana/json_writer.hpp"
#include "../utility/sindy_point2d.h"
#include "IGeGraphic.h"

#define MAKE_POINT_TYPE(pt) \
    {                       \
        (pt).x(), (pt).y()  \
    }

namespace sindy
{
class Point
{
    double _x = 0.0;
    double _y = 0.0;

public:
    Point() {}
    Point(double xx, double yy) : _x(xx), _y(yy) {}

    inline double x() const { return _x; }
    inline void   x(double value) { _x = value; }
    inline double y() const { return _y; }
    inline void   y(double value) { _y = value; }

    Point translate(double xx, double yy) const { return Point(_x + xx, _y + yy); }

    REFLECTION_ALIAS(Point, "Point", FLDALIAS(&Point::_x, "x"), FLDALIAS(&Point::_y, "y"));
};

struct PolyGeometry
{
    virtual ~PolyGeometry() {}
    virtual void jsonObject(std::vector<std::string> &arr) const                                                     = 0;
    virtual void polyList(std::string const &prefix, std::vector<std::pair<std::string, std::string>> &fields) const = 0;

    virtual Point begin() const                                                        = 0;
    virtual Point end() const                                                          = 0;
    virtual void  ends(int segmentCount, std::function<void(Point const &)> fun) const = 0;
};
using PolyGeometrySp = std::shared_ptr<PolyGeometry>;

struct PolySegment : public PolyGeometry
{
public:
    IGeGraphic::ObjectType _type = IGeGraphic::ObjectType::eGeSegmentType;
    Point                  _begin;
    Point                  _end;
    PolySegment() {}
    PolySegment(Point const &begin1, Point const &end1) : _begin(begin1), _end(end1) {}

    void jsonObject(std::vector<std::string> &arr) const override;
    void polyList(std::string const &prefix, std::vector<std::pair<std::string, std::string>> &fields) const override;

    Point begin() const override { return _begin; }
    Point end() const override { return _end; }
    void  ends(int segmentCount, std::function<void(Point const &)> fun) const override { fun(_end); }

    REFLECTION_ALIAS(PolySegment, "PolySegment", FLDALIAS(&PolySegment::_type, "type"),
                     FLDALIAS(&PolySegment::_begin, "begin"), FLDALIAS(&PolySegment::_end, "end"));
};

struct PolyArc : public PolyGeometry
{
    IGeGraphic::ObjectType _type = IGeGraphic::ObjectType::eGeArcType;
    Point                  _center;           // 中心点
    double                 _radius     = 0.0; // 半径w
    double                 _radius2    = 0.0; // 半径h
    double                 _beginAngle = 0.0;
    double                 _sweepAngle = 0.0;
    PolyArc() {}
    PolyArc(Point const &center1, double radius_, double beginAngle1, double sweepAngle1)
        : _center(center1), _radius(radius_), _beginAngle(beginAngle1), _sweepAngle(sweepAngle1)
    {
    }
    PolyArc(Point const &center1, double radius_, double radius_2, double beginAngle1, double sweepAngle1)
        : _center(center1), _radius(radius_), _radius2(radius_2), _beginAngle(beginAngle1), _sweepAngle(sweepAngle1)
    {
    }

    void jsonObject(std::vector<std::string> &arr) const override;
    void polyList(std::string const &prefix, std::vector<std::pair<std::string, std::string>> &fields) const override;

    Point begin() const override;
    Point end() const override;
    void  ends(int segmentCount, std::function<void(Point const &)> fun) const override;

    REFLECTION_ALIAS(PolyArc, "PolyArc", FLDALIAS(&PolyArc::_type, "type"), FLDALIAS(&PolyArc::_center, "center"),
                     FLDALIAS(&PolyArc::_radius, "radius"), FLDALIAS(&PolyArc::_radius2, "radius2"),
                     FLDALIAS(&PolyArc::_beginAngle, "beginAngle"), FLDALIAS(&PolyArc::_sweepAngle, "sweepAngle"));
};

struct Polygon
{
    IGeGraphic::ObjectType type = IGeGraphic::ObjectType::eGePolygonType;
    std::vector<int>       index; // 轮廓边或内洞边的数量，至少存在一个元素

    std::vector<PolyGeometrySp> _elements; // 保存基类指针

    Polygon(std::vector<int> const &index1) : index(index1) {}

    std::string json() const;
};
REFLECTION(Polygon, type, index); // 边要自己添加

using PolygonSp = std::shared_ptr<Polygon>;
} // namespace sindy

#define BOOLEAN_INTERSECTION_SYMBOL "∩"
#define BOOLEAN_UNION_SYMBOL "∪"
#define BOOLEAN_DIFFERENCE_SYMBOL "-"

/*
 * @brief 多边形
 */
class GePolygon : public IGeGraphic
{
    std::vector<int>                   _indexes;  // 轮廓边或内洞边的数量，至少存在一个元素
    std::vector<sindy::PolyGeometrySp> _elements; // 保存基类指针

    VISIT_THIS_CLASS
public:
    GePolygon();
    GePolygon(std::vector<int> const &index);

    QPainterPath shape() const override;
    QRectF       boundingRect() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void drawArcSegment(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // 对象创建相关的业务
    std::unique_ptr<GraphicMaker> subMake() const override { return std::make_unique<PolygonMaker>(); }

    void list(std::vector<std::pair<std::string, std::string>> &fields) const override;

public:
    std::vector<int>  indexes() const { return _indexes; }
    std::vector<int> &indexesRef() { return _indexes; }
    void              indexes(std::vector<int> const &value) { _indexes = value; }

    std::vector<sindy::PolyGeometrySp> elements() const { return _elements; }
    void                               setEdge(sindy::PolyGeometrySp element) { _elements.emplace_back(element); }

    void addNewEdge(sindy::PolyGeometrySp element)
    {
        if (auto pLine = dynamic_cast<sindy::PolySegment *>(element.get()); !pLine)
            this->addSaveFlag(eNonSegmentEdge);
        _elements.emplace_back(element);
    }

    bool isLinestring() const;

    bool getPoints(std::vector<std::vector<sindy::Point>> &output) const;

    static void makePolyClockwise(std::vector<sindy::Point> &pts);
    static bool isPolyClockwise(std::vector<sindy::Point> const &pts);

    static std::string booleanIntersectionName(GePolygon *poly1, GePolygon *poly2);
    static std::string booleanUnionName(std::vector<GePolygon *> const &polys);
    static std::string booleanDifferenceName(std::vector<GePolygon *> const &polys);
};

#endif // !GE_POLYGON_H
