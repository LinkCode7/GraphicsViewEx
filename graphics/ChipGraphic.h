#ifndef CHIP_GRAPHIC_H
#define CHIP_GRAPHIC_H

#include <QColor>
#include <QGraphicsItem>

class ChipGraphic : public QGraphicsItem
{
public:
    ChipGraphic(const QColor &color, int x, int y);

    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget = nullptr) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int              m_x = 0;
    int              m_y = 0;
    QColor           m_color;
    QVector<QPointF> m_stuff;
};

#endif  // CHIP_GRAPHIC_H
