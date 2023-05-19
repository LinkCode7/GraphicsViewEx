#ifndef POLYLINE_DRAG_H
#define POLYLINE_DRAG_H

#include "DragAction.h"

class PolylineDrag : public DragAction
{
public:
    PolylineDrag();

    void onLeftClick(const QPointF& curClick) override;
    void onMove(const QPointF& curMove) override;

    bool isEndDrag(const QPointF& curClick) override;

    bool keyAction(Qt::Key key);
};

#endif
