#ifndef GE_SYMBOL_POINT_H
#define GE_SYMBOL_POINT_H

#include "IGePoint.h"

// 符号点
class GeSymbolPoint : public IGePoint, public GraphicsSymbol
{
    VISIT_THIS_CLASS
public:
    GeSymbolPoint(QPointF const &pt, GraphicsSymbol::Type type = eAimPoint);

    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget = nullptr) override;

    // 对象创建相关的业务
    std::unique_ptr<GraphicMaker> subMake() const override { return std::make_unique<AimMaker>(); }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // !GE_SYMBOL_POINT_H
