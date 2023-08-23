#ifndef DB_GRAPHIC_H
#define DB_GRAPHIC_H

#include <QGraphicsItem>
#include <qstyle>

#include "../data/DataParse.h"
#include "../data/GraphicsMaker.h"
#include "../data/SaveFlags.h"

class GraphicMaker;

#define NOTIFY_MAKE_GRAPHIC() this->notify()

#define VISIT_THIS_CLASS \
public:                  \
    void visit(VisitGraphics *visitor) override { visitor->visit(this); }

class IGeGraphic : public QGraphicsItem
{
public:
    // 可以直接构造的对象
    enum ObjectType : unsigned int
    {
        eUnknownType         = 0,
        eGeBoxType           = 1, // 包围盒
        eChipType            = 2,
        eGeAimType           = 3, // 定位点
        eGePolylineType      = 4, // 连续线段，LineString
        eGePolylineIndexType = 5, // 绘制带索引的连续线段
        eGeSquarePointsType  = 6, // 点集

        eGeSegmentType = 7,  // 线段
        eGeArcType     = 8,  // 弧线
        eGeBezierType  = 9,  // 贝塞尔曲线
        eGePolygonType = 10, // 多边形
    };

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

    void        id(std::string const &id) { _id = id; }
    std::string id() const { return _id; }

    ObjectType objectType() const { return _type; }

    void addFlag(SaveFlags::Flag flag) { _runTimeFlags.add(flag); }
    void removeFlag(SaveFlags::Flag flag) { _runTimeFlags.remove(flag); }
    bool hasFlag(SaveFlags::Flag flag) const { return _runTimeFlags.has(flag); }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    std::string _id;
    QColor      _color;
    ObjectType  _type;

    uint64_t  _saveFlags = 0;
    SaveFlags _runTimeFlags;
};

#endif // !DB_GRAPHIC_H
