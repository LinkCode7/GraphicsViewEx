#ifndef BOX_GRAPHIC_H
#define BOX_GRAPHIC_H

#include "BasicGraphic.h"

class BoxGraphic : public BasicGraphic
{
public:
    BoxGraphic(QRectF const &rect);
    BoxGraphic(double minX, double minY, double maxX, double maxY);

    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget = nullptr) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QRectF m_rect;
};

#endif  // !BOX_GRAPHIC_H
