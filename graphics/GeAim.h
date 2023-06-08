#ifndef GE_AIM_H
#define GE_AIM_H

#include "IGePoint.h"

// 定位点
class GeAim : public IGePoint
{
    VISIT_THIS_CLASS
public:
    GeAim(QPointF const &pt);

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

#endif // !GE_AIM_H
