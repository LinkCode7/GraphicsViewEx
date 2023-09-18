#ifndef POLYLINE_DRAG_H
#define POLYLINE_DRAG_H

#include "DragAction.h"

class PolylineDrag : public DragAction
{
public:
    IGeGraphic* makeGeometry() override;
    void        onLeftClick(const QPointF& curClick) override;
    void        onMouseMove(const QPointF& curMove) override;

    void onRightClick(const QPointF& curClick) override;

    void onKeyDown(Qt::Key key);
};

class PolygonDrag : public DragAction
{
public:
    IGeGraphic* makeGeometry() override;
    void        onLeftClick(const QPointF& curClick) override;
    void        onMouseMove(const QPointF& curMove) override;

    void onRightClick(const QPointF& curClick) override;

    void onKeyDown(Qt::Key key);
};

#endif
