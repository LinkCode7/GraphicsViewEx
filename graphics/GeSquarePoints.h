#ifndef GE_SQUARE_POINTS_H
#define GE_SQUARE_POINTS_H

#include <QGraphicsItem>
#include <algorithm>

#include "IGePointSet.h"

// 方形点集
class GeSquarePoints : public IGePointSet
{
    VISIT_THIS_CLASS
public:
    GeSquarePoints();
    GeSquarePoints(std::initializer_list<QPointF> const &pts);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // 对象创建相关的业务
    std::unique_ptr<GraphicMaker> subMake() const override { return std::make_unique<GeSquarePointsMaker>(); }

public:
};

#endif // !GE_SQUARE_POINTS_H