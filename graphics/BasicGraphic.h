#ifndef BASIC_GRAPHIC_H
#define BASIC_GRAPHIC_H

#include <QGraphicsItem>
#include <qstyle>

class BasicGraphic : public QGraphicsItem
{
public:
    BasicGraphic();
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget = nullptr) override;

    void setColor(QColor const &color) { m_color = color; }

    QColor getDrawColor(QStyle::State state);
    double getDrawWidth(QStyle::State state);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    std::string m_id;
    QColor      m_color;
};

#endif  // !BASIC_GRAPHIC_H
