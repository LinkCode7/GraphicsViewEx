#ifndef DB_GRAPHIC_H
#define DB_GRAPHIC_H

#include <QGraphicsItem>
#include <qstyle>

#include "../data/DataParse.h"
#include "../data/GraphicsMaker.h"
#include "../data/SaveFlags.h"

class GraphicMaker;

#define VISIT_THIS_CLASS                        \
public:                                         \
    void visit(VisitGraphics *visitor) override \
    {                                           \
        visitor->visit(this);                   \
    }

class IGeGraphic : public QGraphicsItem
{
public:
    // 可以直接构造的对象，和sindyk::NodeType对应
    enum ObjectType : unsigned int
    {
        eUnknownType        = 0,
        eGePolylineType     = 1, // 连续线段，LineString
        eGePolygonType      = 2, // 多边形
        eGeSegmentType      = 3, // 线段
        eGeArcType          = 4, // 弧线
        eGeBezierType       = 5, // 贝塞尔曲线
        eGeBoxType          = 6, // 包围盒
        eGeSymbolPointType  = 7, // 符号点
        eGeSquarePointsType = 8, // 点集
        eChipType           = 1001,
    };

    enum ObjectStatus : unsigned int
    {
        eUnknownStatus  = 0,
        eNonSegmentEdge = 1, // 多边形中存在非线段边
    };

    virtual ~IGeGraphic() {}
    IGeGraphic(ObjectType type);

    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget = nullptr) override;

    // 请在子类的构造函数中调用make函数
    void notify() { subMake()->receive(this); }
    // GraphicMaker的子类，用来完成对象创建相关的业务
    virtual std::unique_ptr<GraphicMaker> subMake() const = 0;

    // 访问者模式
    virtual void visit(VisitGraphics *visitor) = 0;

    virtual void list(std::vector<std::string> const &) {}

public:
    uint   getArgb32() const { return _color.rgba64().toArgb32(); }
    uint   setArgb32(uint argb) { _color = QRgba64::fromArgb32(argb); }
    QColor getGeColor() const { return _color; }
    void   setGeColor(QColor const &color) { _color = color; }

    QColor getDrawColor(QStyle::State state);
    double getDrawWidth(QStyle::State state);

    uint32_t id() const { return _id; }
    void     id(uint32_t id) { _id = id; }

    std::string name() const { return _name; }
    void        name(std::string const &value) { _name = value; }

    uint32_t saveStatus() const { return _saveFlag; }
    void     saveStatus(uint32_t value) { _saveFlag = value; }

    ObjectType objectType() const { return _type; }

    void addFlag(SaveFlags::Flag flag) { _runTimeFlags.add(flag); }
    void removeFlag(SaveFlags::Flag flag) { _runTimeFlags.remove(flag); }
    bool hasFlag(SaveFlags::Flag flag) const { return _runTimeFlags.has(flag); }

    void addStatus(ObjectStatus state) { _saveFlag |= state; }
    void removeStatus(ObjectStatus state) { _saveFlag &= (~state); }
    bool hasStatus(ObjectStatus state) const { return _saveFlag & state; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    ObjectType  _type;
    QColor      _color;
    uint32_t    _id;
    std::string _name;
    uint32_t    _saveFlag = 0;

    SaveFlags _runTimeFlags;
};

#endif // !DB_GRAPHIC_H
