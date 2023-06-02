#include <QGraphicsItem>
#include <algorithm>

#include "BasicGraphic.h"

class PolylineGraphic : public BasicGraphic
{
    VISIT_THIS_CLASS
public:
    PolylineGraphic();
    PolylineGraphic(std::initializer_list<QPointF> const &list);
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // 对象创建相关的业务
    std::unique_ptr<GraphicMaker> subMake() const override { return std::make_unique<PolylineMaker>(); }

public:
    void addPoint(const QPointF &ptNow);
    void setLastPt(const QPointF &ptNow);
    bool getFirstPt(QPointF &pt) const;

    void closePolyLine(const QPointF &point);

    bool checkCross(const QPointF &point);

    bool collisionDect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);

    bool crossDect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);

    bool onLineDect(const QPointF &p, const QPointF &p1, const QPointF &p2);

    QVector<QPointF> getPoints();
    void             setPoints(QVector<QPointF> const &pts) { _points = pts; }

private:
    QVector<QPointF> _points;
};