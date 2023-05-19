#include "PolylineGraphic.h"

// ÍÏ×§Àà
class DragAction
{
public:
    DragAction(const QPointF& firstClick);
    PolylineGraphic* getPolyline();
    void             onLeftClick(const QPointF& curClick);
    void             onMove(const QPointF& curMove);
    bool             keyAction(int keyNum, int& curNum);

private:
    PolylineGraphic* m_ploy;
};

#include <memory>
using DragActionSP = std::shared_ptr<DragAction>;