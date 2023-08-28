#include "GraphicsDocument.h"

#include <set>

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

void Document::deleteSelectedItems()
{
    std::set<IGeGraphic*> select;
    for (auto const& item : _selected)
    {
        select.emplace(item);
    }

    std::vector<IGeGraphic*> unselect;

    auto pScene = GeArchive().scene();
    for (auto const& item : _geometries)
    {
        if (select.find(item) != select.end())
        {
            pScene->removeItem(item);
            delete item;
            continue;
        }

        unselect.emplace_back(item);
    }

    _selected.clear();
    _geometries = std::move(unselect);
}
