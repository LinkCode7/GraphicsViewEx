#ifndef GE_POLYLINE_INDEX_H
#define GE_POLYLINE_INDEX_H

#include <QGraphicsItem>
#include <algorithm>

#include "IGePointSet.h"

// 方形点集
class GePolylineIndex : public IGePointSet
{
    VISIT_THIS_CLASS
public:
    GePolylineIndex();
    GePolylineIndex(std::initializer_list<QPointF> const &list);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // 对象创建相关的业务
    std::unique_ptr<GraphicMaker> subMake() const override { return std::make_unique<GePolylineIndexMaker>(); }

public:
};

#endif // !GE_POLYLINE_INDEX_H