#include "GraphicsMaker.h"

#include "../data/GraphicsDocument.h"
#include "../graphics/BoxGraphic.h"
#include "../graphics/PointGraphic.h"
#include "../graphics/PolylineGraphic.h"
#include "../graphics/chip.h"

void GraphicMaker::receive(BasicGraphic *pNew)
{
    addToDocument(pNew);

    subCreate(pNew);
}

void GraphicMaker::addToDocument(BasicGraphic *pNew)
{
    Document::instance().addGraphic(pNew);
}
