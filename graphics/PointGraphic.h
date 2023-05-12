#ifndef POINT_GRAPHIC_H
#define POINT_GRAPHIC_H

#include "BasicGraphic.h"

class PointGraphic : public BasicGraphic
{
public:
    PointGraphic(double x, double y);
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget = nullptr) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    QPointF m_pt;
};

#endif  // !POINT_GRAPHIC_H
