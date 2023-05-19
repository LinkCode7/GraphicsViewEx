#ifndef DRAG_ACTION_H
#define DRAG_ACTION_H

#include <memory>

#include "qpoint.h"

class BasicGraphic;

class DragAction
{
public:
    DragAction() {}

    virtual BasicGraphic* geometry() { return m_geo; }

    virtual void onLeftClick(const QPointF& curClick) {}
    virtual void onMove(const QPointF& curMove) {}

    // 返回是否结束拖拽
    virtual bool keyAction(Qt::Key key) { return false; }

public:
    virtual bool isEndDrag(const QPointF& curClick) { return false; }

protected:
    BasicGraphic* m_geo = nullptr;
};
using DragActionSp = std::shared_ptr<DragAction>;

#endif
