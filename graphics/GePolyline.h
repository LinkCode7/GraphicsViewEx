#ifndef GE_POLYLINE_H
#define GE_POLYLINE_H

#include "IGePointSet.h"

class GePolyline : public IGePointSet, public GraphicsSymbol
{
    VISIT_THIS_CLASS
public:
    GePolyline();
    GePolyline(std::initializer_list<QPointF> const &pts, GraphicsSymbol::Type type = eUnknownSymbol);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // 对象创建相关的业务
    std::unique_ptr<GraphicMaker> subMake() const override { return std::make_unique<PolylineMaker>(); }

public:
    void setLastPt(const QPointF &ptNow);
    bool getFirstPt(QPointF &pt) const;

    void closePolyline(const QPointF &point);

    // 避免自相交
    bool checkCross(const QPointF &point);

    bool collisionDect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);

    bool crossDect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);

    bool onLineDect(const QPointF &p, const QPointF &p1, const QPointF &p2);
};

#endif // !GE_POLYLINE_H