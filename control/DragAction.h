#ifndef DRAG_ACTION_H
#define DRAG_ACTION_H

#include <memory>

#include "qpoint.h"

class IGeGraphic;

class DragAction
{
public:
    DragAction() {}

    virtual IGeGraphic* makeGeometry() = 0;
    virtual IGeGraphic* geometry() { return m_geo; }

    virtual void onLeftClick(const QPointF& curClick) {}
    virtual void onMouseMove(const QPointF& curMove) {}
    virtual void onRightClick(const QPointF& curClick) {}

    virtual void onKeyDown(Qt::Key key) {}

    enum Flag : unsigned int
    {
        eUnknownFlag   = 0,
        eEndDragAction = 1, //
    };
    void addFlag(Flag flag) { _flag |= flag; }
    void removeFlag(Flag flag) { _flag &= (~flag); }
    bool hasFlag(Flag flag) const { return _flag & flag; }

protected:
    uint32_t    _flag = 0;
    IGeGraphic* m_geo = nullptr;
};
using DragActionSp = std::shared_ptr<DragAction>;

#endif
