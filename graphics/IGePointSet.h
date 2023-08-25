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

class GraphicsSymbol
{
public:
    enum Type
    {
        eUnknownSymbol = 0,
        eAimPoint      = 1, // 瞄准镜
        ePointsIndex   = 2, // 多段线端点的序列号
    };
    Type symbolType() const { return _symbol; }
    void symbolType(Type value) { _symbol = value; }

    GraphicsSymbol(Type symbol) : _symbol(symbol) {}

protected:
    Type _symbol = eUnknownSymbol;
};

#endif // !I_GE_POINT_SET_H