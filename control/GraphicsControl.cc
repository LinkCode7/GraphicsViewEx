#include "GraphicsControl.h"

#include "../core/GraphicsView.h"
#include "../data/DataParse.h"
#include "../data/DataSave.h"
#include "../graphics/GeBox.h"
#include "DragAction.h"
#include "PolylineDrag.h"
#include "qevent.h"
#include "qgraphicsscene.h"

extern QGraphicsScene* g_pScene;

void sindy::viewKeyDown(QKeyEvent* event, GraphicsView* view)
{
    if (!view)
        return;

    const auto& key = event->key();

    auto drag = view->drag();
    if (drag)
    {
        if (drag->keyAction(static_cast<Qt::Key>(key)))
        {
            view->setDrag(nullptr);
            // view->removeState(GraphicsView::eDragging);
        }
    }

    switch (key)
    {
        case Qt::Key_P:
        {
            drag = std::make_shared<PolylineDrag>();
            view->setDrag(drag);

            view->setCommand("polyline");
            view->addState(GraphicsView::eDragInit);
            // view->addState(GraphicsView::eDragging);
            break;
        }
        case Qt::Key_B:
        {
            auto items = g_pScene->selectedItems();
            for (auto const& item : items)
            {
                QRectF rectf = item->boundingRect();
                auto   mat   = item->sceneMatrix();
                rectf.translate({mat.dx(), mat.dy()});

                auto pItem = new GeBox(rectf);
                pItem->setColor({255, 235, 0});
                pItem->setFlags(0);
                g_pScene->addItem(pItem);
            }
            break;
        }
        case Qt::Key_Delete:
        {
            if (!g_pScene)
                return;

            auto items = g_pScene->selectedItems();

            for (const auto& item : items)
                g_pScene->removeItem(item);

            break;
        }
        case Qt::Key_S:
        {
            if (event->modifiers() & Qt::ControlModifier)
            {
                SaveFlags flag;
                flag.full(true);

                SaveGraphicsData data;
                data.saveAsFile(view, flag);
            }
            break;
        }
        case Qt::Key_R: // reset
        {
            std::string triangle =
                "010002312e3100037f00000000007f000000000004020501010402f7ecd9b307030007048700002486000058870000bd870000348700000b870000a387"
                "000024860000580000";

            auto test =
                "010002312e3100037f0000000000007f0000000000007f00000004010503010102c08183fe0f0300047f0000000000007f0000000000007f0000000585"
                "00009085000090870000908700002c000103028080fcff0f0300047f0000000000007f0000000000007f0000000686000090850100900001040280fe83"
                "f80f0300047f0000000000007f0000000000007f000000070b0084703dba85cccc1c84703dba85cccc7400850ad7cc84703dba86ecd17484703dba86b8"
                "9e018590c26586b89e2d86a4702185cccc7485703dba84cccc1c86a4702184cccccc8590c2650084703dba0000";

            ParseGraphicsData data;
            // data.reset(view, test);
            data.fromFile(view, DOCUMENT_SAVE_FILE_NAME);
            break;
        }
        default:
            break;
    }
}
