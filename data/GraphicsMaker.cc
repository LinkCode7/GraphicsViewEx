#include "GraphicsMaker.h"

#include "../data/GraphicsDocument.h"
#include "../graphics/GeAim.h"
#include "../graphics/GeBox.h"
#include "../graphics/GePolyline.h"
#include "../graphics/chip.h"

void GraphicMaker::receive(IGeGraphic *pNew)
{
    addToDocument(pNew);

    subCreate(pNew);
}

void GraphicMaker::addToDocument(IGeGraphic *pNew)
{
    Document::instance().addGraphic(pNew);
}
