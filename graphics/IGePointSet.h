#ifndef I_GE_POINT_SET_H
#define I_GE_POINT_SET_H

#include <QGraphicsItem>
#include <algorithm>

#include "IGeGraphic.h"

class IGePointSet : public IGeGraphic
{
public:
    IGePointSet(IGeGraphic::ObjectType type);
    IGePointSet(std::initializer_list<QPointF> const &list, ObjectType type);

    QRectF       boundingRect() const override;
    QPainterPath shape() const override;

public:
    std::vector<QPointF>        points() { return _points; }
    const std::vector<QPointF> *getPoints() const { return &_points; }
    void                        setPoints(std::vector<QPointF> const &pts) { _points = pts; }

    void addPoint(const QPointF &ptNow) { _points.emplace_back(ptNow); }

protected:
    std::vector<QPointF> _points;
};

#endif