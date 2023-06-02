#ifndef POINT_GRAPHIC_H
#define POINT_GRAPHIC_H

#include "BasicGraphic.h"

class PointGraphic : public BasicGraphic
{
    VISIT_THIS_CLASS
public:
    PointGraphic(QPointF const &pt);
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget = nullptr) override;

    // 对象创建相关的业务
    std::unique_ptr<GraphicMaker> subMake() const override { return std::make_unique<PointMaker>(); }

public:
    QPointF point() const { return _pt; }
    void    point(QPointF const &pt) { _pt = pt; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    QPointF _pt;
};

#endif // !POINT_GRAPHIC_H
