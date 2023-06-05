#ifndef GE_BOX_H
#define GE_BOX_H

#include "IGeGraphic.h"

class GeBox : public IGeGraphic
{
    VISIT_THIS_CLASS
public:
    GeBox(QRectF const &rect);
    GeBox(QPointF const &ptMin, QPointF const &ptMax);

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

#endif // !GE_BOX_H
