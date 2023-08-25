#include "GraphicsDocument.h"

#include "../graphics/IGeGraphic.h"
#include "../view/GraphicsArchive.h"
#include "../view/GraphicsScene.h"

void Document::reset()
{
    auto pScene = GeArchive().scene();
    for (auto& item : _geometries)
    {
        pScene->removeItem(item);
        delete item;
    }

    _geometries.clear();
}

void Document::addGraphic(IGeGraphic* pGraphic)
{
    if (!pGraphic)
        return;
    _geometries.emplace_back(pGraphic);
}
bool Document::removeGraphic(IGeGraphic* pGraphic)
{
    for (auto iter = _geometries.begin(); iter != _geometries.end();)
    {
        if (*iter == pGraphic)
        {
            iter = _geometries.erase(iter);
            return true;
        }
        else
        {
            ++iter;
        }
    }
    return false;
}
