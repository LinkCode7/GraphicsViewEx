#include "PolylineDrag.h"

#include "../graphics/GePolyline.h"
#include "../utility/utility.h"
#include "qgraphicsscene.h"

extern QGraphicsScene* g_pScene;

PolylineDrag::PolylineDrag()
{
    m_geo = new GePolyline();
    g_pScene->addItem(m_geo);
}

void PolylineDrag::onLeftClick(const QPointF& curClick)
{
    auto poly = static_cast<GePolyline*>(m_geo);
    if (poly->checkCross(curClick))
        return;

    poly->addPoint(curClick);
}

bool PolylineDrag::isEndDrag(const QPointF& curClick)
{
    auto poly = static_cast<GePolyline*>(m_geo);
    return !poly->checkCross(curClick);
}

void PolylineDrag::onMove(const QPointF& curMove)
{
    static_cast<GePolyline*>(m_geo)->setLastPt(curMove);
}

bool PolylineDrag::keyAction(Qt::Key key)
{
    if (key == Qt::Key_Escape)
    {
        g_pScene->removeItem(m_geo);
        return true;
    }
    else if (key == Qt::Key_C)
    {
        QPointF pt;
        auto    poly = static_cast<GePolyline*>(m_geo);
        if (poly->getFirstPt(pt))
        {
            poly->closePolyline(pt);
            return true;
        }
    }
    return false;
}
