#include "GraphicsDocument.h"

#include "../core/GraphicsView.h"
#include "../graphics/IGeGraphic.h"
#include "qgraphicsscene.h"

extern QGraphicsScene* g_pScene;

void Document::reset()
{
    if (!g_pScene)
        return;

    for (auto const& item : _mapId2Geometry)
    {
        g_pScene->removeItem(item.second);
    }

    _newId = 0;
    _mapId2Geometry.clear();
}

void Document::calculateNewId()
{
    uint max = 0;
    for (auto const& item : _mapId2Geometry)
    {
        auto id = item.second->id();
        if (id > max)
            max = id;
    }

    _newId = max + 1;
}

void Document::addGraphic(IGeGraphic* pGraphic)
{
    if (!pGraphic)
        return;
    _mapId2Geometry[_newId++] = pGraphic;
}
bool Document::removeGraphic(IGeGraphic* pGraphic)
{
    for (auto iter = _mapId2Geometry.begin(); iter != _mapId2Geometry.end();)
    {
        if (iter->second == pGraphic)
        {
            iter = _mapId2Geometry.erase(iter);
            return true;
        }
        else
        {
            ++iter;
        }
    }
    return false;
}

void Document::getObjects(std::vector<IGeGraphic*>& arrObject)
{
    for (auto const& item : _mapId2Geometry)
        arrObject.emplace_back(item.second);
}