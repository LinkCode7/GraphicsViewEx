#include "GraphicsArchive.h"

#include "../graphics/IGeGraphic.h"

void GraphicsArchive::addCustomItem(IGeGraphic* pItem)
{
    _scene->addItem(pItem);
    _doc.addGraphic(pItem);
}
