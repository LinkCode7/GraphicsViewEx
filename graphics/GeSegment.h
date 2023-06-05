#ifndef GE_SEGMENT_H
#define GE_SEGMENT_H

#include <QGraphicsItem>
#include <algorithm>

#include "IGePointSet.h"

// 线段
class GeSegment : public IGePointSet
{
    VISIT_THIS_CLASS
public:
    GeSegment();
    GeSegment(QPointF const &from, QPointF const &to);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // 对象创建相关的业务
    std::unique_ptr<GraphicMaker> subMake() const override { return std::make_unique<GeSegmentMaker>(); }

public:
    QPointF beginPoint() const;
    void    beginPoint(QPointF const &pt);

    QPointF endPoint() const;
    void    endPoint(QPointF const &pt);
};

#endif // !GE_SEGMENT_H