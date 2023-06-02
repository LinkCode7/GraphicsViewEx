#ifndef BASIC_GRAPHIC_H
#define BASIC_GRAPHIC_H

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

class BasicGraphic : public QGraphicsItem
{
public:
    enum ObjectType : unsigned int
    {
        eUnknownType  = 0,
        eBoxType      = 1,
        eChipType     = 2,
        ePointType    = 3,
        ePolylineType = 4,
    };

    BasicGraphic(ObjectType type);

    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget = nullptr) override;

    // 请在子类的构造函数中调用make函数
    void notify() { subMake()->receive(this); }
    // GraphicMaker的子类，用来完成对象创建相关的业务
    virtual std::unique_ptr<GraphicMaker> subMake() const = 0;

    // 访问者模式
    virtual void visit(VisitGraphics *visitor) = 0;

public:
    int  getRgba64() const { return _color.rgba64(); }
    void setColor(QColor const &color) { _color = color; }

    QColor getDrawColor(QStyle::State state);
    double getDrawWidth(QStyle::State state);

    void id(uint id) { _id = id; }
    uint id() const { return _id; }

    ObjectType objectType() const { return _type; }

    void addFlag(SaveFlags::Flag flag) { _saveFlags.add(flag); }
    void removeFlag(SaveFlags::Flag flag) { _saveFlags.remove(flag); }
    bool hasFlag(SaveFlags::Flag flag) const { return _saveFlags.has(flag); }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    uint       _id{0};
    QColor     _color;
    ObjectType _type;

    SaveFlags _saveFlags;
};

#endif // !BASIC_GRAPHIC_H
