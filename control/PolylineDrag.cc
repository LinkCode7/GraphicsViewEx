#include "PolylineDrag.h"

#include "../graphics/GePolygon.h"
#include "../graphics/GePolyline.h"
#include "../utility/utility.h"
#include "../view/GraphicsArchive.h"

IGeGraphic* PolylineDrag::makeGeometry()
{
    return new GePolyline();
}

void PolylineDrag::onLeftClick(const QPointF& curClick)
{
    if (!m_geo)
    {
        m_geo = this->makeGeometry();
        GeArchive().addCustomItem(m_geo);
    }

    auto poly = static_cast<GePolyline*>(m_geo);
    if (poly->checkCross(curClick))
        return;

    poly->addPoint(curClick);
}

void PolylineDrag::onRightClick(const QPointF& curClick)
{
    if (!m_geo)
        return;

    addFlag(DragAction::eEndDragAction);
}

void PolylineDrag::onMouseMove(const QPointF& curMove)
{
    if (m_geo)
        static_cast<GePolyline*>(m_geo)->setLastPt(curMove);
}

void PolylineDrag::onKeyDown(Qt::Key key)
{
    if (key == Qt::Key_Escape)
    {
        GeArchive().doc()->removeGraphic(m_geo);
        addFlag(DragAction::eEndDragAction);
    }
    else if (key == Qt::Key_C)
    {
        QPointF pt;
        auto    poly = static_cast<GePolyline*>(m_geo);
        if (poly->getFirstPt(pt))
        {
            poly->closePolyline(pt);
            addFlag(DragAction::eEndDragAction);
        }
    }
}

IGeGraphic* PolygonDrag::makeGeometry()
{
    return new GePolygon();
}

void PolygonDrag::onLeftClick(const QPointF& curClick)
{
    if (!m_geo)
    {
        m_geo = this->makeGeometry();
        GeArchive().addCustomItem(m_geo);
    }

    auto poly = static_cast<GePolygon*>(m_geo);
    if (poly->selfIntersection(MAKE_POINT_TYPE(curClick)))
        return;

    poly->addEdgeByPoint(MAKE_POINT_TYPE(curClick), true);
}

void PolygonDrag::onRightClick(const QPointF& curClick)
{
    if (!m_geo)
        return;

    addFlag(DragAction::eEndDragAction);
}

void PolygonDrag::onMouseMove(const QPointF& curMove)
{
    if (auto geo = static_cast<GePolygon*>(m_geo); geo)
        geo->setLastEdgeEndPoint(MAKE_POINT_TYPE(curMove), true);
}

void PolygonDrag::onKeyDown(Qt::Key key)
{
    if (key == Qt::Key_Escape)
    {
        GeArchive().doc()->removeGraphic(m_geo);
        addFlag(DragAction::eEndDragAction);
    }
    else if (key == Qt::Key_C)
    {
        auto poly = static_cast<GePolygon*>(m_geo);
        if (poly->setClose())
            addFlag(DragAction::eEndDragAction);
    }
}