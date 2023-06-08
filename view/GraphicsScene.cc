#include "GraphicsScene.h"

#include "../graphics/IGeGraphic.h"
#include "GraphicsArchive.h"
#include "PropertyTable.h"

GraphicsScene::GraphicsScene(QObject* parent) : QGraphicsScene(parent)
{
    connect(this, &QGraphicsScene::selectionChanged, this, &GraphicsScene::selectionChanged);
}

void GraphicsScene::selectionChanged()
{
    auto doc = GeArchive().doc();
    doc->clearSelectedGraphics();

    auto list = this->selectedItems();
    for (auto const& item : list)
    {
        auto pGraphic = dynamic_cast<IGeGraphic*>(item);
        if (!pGraphic)
            continue;

        doc->addSelectedGraphics(pGraphic);
    }

    _property->doSelectionChanged();
}