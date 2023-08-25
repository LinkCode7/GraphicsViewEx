#include "GraphicsMaker.h"

#include "../graphics/GeBox.h"
#include "../graphics/GePolyline.h"
#include "../graphics/GeSymbolPoint.h"
#include "../graphics/chip.h"
#include "../view/GraphicsArchive.h"

void GraphicMaker::receive(IGeGraphic *pNew)
{
    addToDocument(pNew);

    subCreate(pNew);
}

void GraphicMaker::addToDocument(IGeGraphic *pNew)
{
    GeArchive().doc()->addGraphic(pNew);
}
