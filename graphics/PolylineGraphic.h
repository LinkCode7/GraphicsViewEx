#include <QGraphicsItem>
#include <algorithm>

#include "BasicGraphic.h"

// ªÊ÷∆∂‡∂Œœﬂ
class PolylineGraphic : public BasicGraphic
{
public:
    PolylineGraphic(double minX, double minY, double maxX, double maxY);
    PolylineGraphic(const QPointF &ptBegin);
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget = nullptr) override;
    void         addPoint(const QPointF &ptNow);
    void         setLastPt(const QPointF &ptNow);

    bool closePolyLine(const QPointF &point);

    bool checkCross(const QPointF &point);

    bool collisionDect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);

    bool crossDect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);

    bool onLineDect(const QPointF &p, const QPointF &p1, const QPointF &p2);

    QVector<QPointF> getPoints();

protected:
    QVector<QPointF> m_points;
};