#ifndef GE_POLYGON_H
#define GE_POLYGON_H

#include "../iguana/iguana/json_writer.hpp"
#include "IGeGraphic.h"

#define MAKE_POINT_TYPE(pt) \
    {                       \
        pt.x, pt.y          \
    }
#define MAKE_POINT_TYPE2(pt) \
    {                        \
        pt.x(), pt.y()       \
    }

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

struct PolyGeometry
{
    virtual ~PolyGeometry() {}
    virtual void jsonObject(std::vector<std::string> &arr) const                                                     = 0;
    virtual void polyList(std::string const &prefix, std::vector<std::pair<std::string, std::string>> &fields) const = 0;
};
using PolyGeometrySp = std::shared_ptr<PolyGeometry>;

struct PolySegment : public PolyGeometry
{
public:
    IGeGraphic::ObjectType type = IGeGraphic::ObjectType::eGeSegmentType;
    Point                  begin;
    Point                  end;
    PolySegment() {}
    PolySegment(Point const &begin1, Point const &end1) : begin(begin1), end(end1) {}

    void jsonObject(std::vector<std::string> &arr) const override;
    void polyList(std::string const &prefix, std::vector<std::pair<std::string, std::string>> &fields) const override;
};
REFLECTION(PolySegment, type, begin, end);

struct PolyArc : public PolyGeometry
{
    IGeGraphic::ObjectType type = IGeGraphic::ObjectType::eGeArcType;
    Point                  center;           // 中心点
    double                 radius     = 0.0; // 半径w
    double                 radius2    = 0.0; // 半径h
    double                 beginAngle = 0.0;
    double                 sweepAngle = 0.0;
    PolyArc() {}
    PolyArc(Point const &center1, double radius_, double beginAngle1, double sweepAngle1)
        : center(center1), radius(radius_), beginAngle(beginAngle1), sweepAngle(sweepAngle1)
    {
    }
    PolyArc(Point const &center1, double radius_, double radius_2, double beginAngle1, double sweepAngle1)
        : center(center1), radius(radius_), radius2(radius_2), beginAngle(beginAngle1), sweepAngle(sweepAngle1)
    {
    }

    void jsonObject(std::vector<std::string> &arr) const override;
    void polyList(std::string const &prefix, std::vector<std::pair<std::string, std::string>> &fields) const override;
};
REFLECTION(PolyArc, type, center, radius, radius2, beginAngle, sweepAngle);

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
    std::vector<int> indexes() const { return _indexes; }
    void             indexes(std::vector<int> const &value) { _indexes = value; }

    std::vector<sindy::PolyGeometrySp> elements() const { return _elements; }
    void                               setEdge(sindy::PolyGeometrySp element) { _elements.emplace_back(element); }

    void addNewEdge(sindy::PolyGeometrySp element)
    {
        if (auto pLine = dynamic_cast<sindy::PolySegment *>(element.get()); !pLine)
            this->addSaveFlag(eNonSegmentEdge);
        _elements.emplace_back(element);
    }

    bool isLinestring() const;
};

#endif // !GE_POLYGON_H
