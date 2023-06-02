#ifndef BOX_GRAPHIC_H
#define BOX_GRAPHIC_H

#include "BasicGraphic.h"

class BoxGraphic : public BasicGraphic
{
    VISIT_THIS_CLASS
public:
    BoxGraphic(QRectF const &rect);
    BoxGraphic(QPointF const &ptMin, QPointF const &ptMax);

    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    // 对象创建相关的业务
    std::unique_ptr<GraphicMaker> subMake() const override { return std::make_unique<BoxMaker>(); }

public:
    QRectF rect() const { return m_rect; }
    void   rect(QRectF const &rect) { m_rect = rect; }

private:
    QRectF m_rect;
};

#endif // !BOX_GRAPHIC_H
