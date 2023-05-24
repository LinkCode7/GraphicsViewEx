#include "GraphicsControl.h"

#include "../core/GraphicsView.h"
#include "../graphics/BoxGraphic.h"
#include "DragAction.h"
#include "PolylineDrag.h"
#include "qgraphicsscene.h"

extern QGraphicsScene* g_pScene;

void sindy::viewKeyDown(Qt::Key key, GraphicsView* view)
{
    if (!view)
        return;

    auto drag = view->drag();
    if (drag)
    {
        if (drag->keyAction(static_cast<Qt::Key>(key)))
        {
            view->setDrag(nullptr);
            // view->removeState(GraphicsView::eDragging);
        }
    }

    if (key == Qt::Key_P)
    {
        drag = std::make_shared<PolylineDrag>();
        view->setDrag(drag);

        view->setCommand("polyline");
        view->addState(GraphicsView::eDragInit);
        // view->addState(GraphicsView::eDragging);
    }
    if (key == Qt::Key_B)
    {
        // QRectF all;
        auto items = g_pScene->selectedItems();
        for (auto const& item : items)
        {
            QRectF rectf = item->boundingRect();
            rectf.translate(item->pos());

            auto pItem = new BoxGraphic(rectf);
            pItem->setColor({255, 235, 0});
            pItem->setFlags(0);
            g_pScene->addItem(pItem);
        }
    }
    else if (key == Qt::Key_Delete)
    {
        if (!g_pScene)
            return;

        auto items = g_pScene->selectedItems();

        for (const auto& item : items) g_pScene->removeItem(item);
    }
}
