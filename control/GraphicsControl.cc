#include "GraphicsControl.h"

#include "../data/DataParse.h"
#include "../data/DataSave.h"
#include "../graphics/GeBox.h"
#include "../view/GraphicsArchive.h"
#include "../view/GraphicsView.h"
#include "DragAction.h"
#include "PolylineDrag.h"
#include "qevent.h"

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

    auto pScene = GeArchive().scene();
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
            auto items = pScene->selectedItems();
            for (auto const& item : items)
            {
                QRectF rectf = item->boundingRect();
                auto   mat   = item->sceneMatrix();
                rectf.translate({mat.dx(), mat.dy()});

                auto pItem = new GeBox(rectf);
                pItem->setGeColor({255, 235, 0});
                pItem->setFlags(0);
                pScene->addItem(pItem);
            }
            break;
        }
        case Qt::Key_Delete:
        {
            if (!pScene)
                return;

            auto items = pScene->selectedItems();

            for (const auto& item : items)
                pScene->removeItem(item);

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
                "010002312e3100037f0000000000007f0000000000007f00000004010506010102c08183fe0f0300047f0000000000007f0000000000007f0000000585"
                "000090850000908700002c86000090000103028080fcff0f0300047f0000000000007f0000000000007f0000000601860000908501009000010702c489"
                "93fe0f0300047f0000000000007f0000000000007f00000006028501009085010090850000908500009000010402c48993fe0f0300047f000000000000"
                "7f0000000000007f000000060b0084703dba85cccc1c84703dba85cccc7400850ad7cc84703dba86ecd17484703dba86b89e018590c26586b89e2d86a4"
                "702185cccc7485703dba84cccc1c86a4702184cccccc8590c2650084703dba00010502c48993fe0f0300047f0000000000007f0000000000007f000000"
                "060b0084703dba85cccc1c84703dba85cccc7400850ad7cc84703dba86ecd17484703dba86b89e018590c26586b89e2d86a4702185cccc7485703dba84"
                "cccc1c86a4702184cccccc8590c2650084703dba00010602ff8180f80f0300047f0000000000007f0000000000007f000000060b0084703dba85cccc1c"
                "84703dba85cccc7400850ad7cc84703dba86ecd17484703dba86b89e018590c26586b89e2d86a4702185cccc7485703dba84cccc1c86a4702184cccccc"
                "8590c2650084703dba0000";

            ParseGraphicsData data;
            // data.reset(view, test);
            data.fromFile(view, DOCUMENT_SAVE_FILE_NAME);
            break;
        }
        default:
            break;
    }
}
