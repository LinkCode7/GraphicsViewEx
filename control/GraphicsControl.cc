#include "GraphicsControl.h"

#include <QDir>
#include <QFileDialog>

#include "..//utility/JsonData.h"
#include "../data/DataParse.h"
#include "../data/DataSave.h"
#include "../graphics/GeBox.h"
#include "../graphics/GePolygon.h"
#include "../iguana/iguana/json_reader.hpp"
#include "../utility/utility.h"
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
                "010002312e3100037f00000000007f000000000004020501010402f7ecd9b307030007048700002486000058870000bd87000034870"
                "0000b870000a387"
                "000024860000580000";

            auto test =
                "010002312e3100037f0000000000007f0000000000007f00000004010506010102c08183fe0f0300047f0000000000007f000000000"
                "0007f0000000585"
                "000090850000908700002c86000090000103028080fcff0f0300047f0000000000007f0000000000007f00000006018600009085010"
                "09000010702c489"
                "93fe0f0300047f0000000000007f0000000000007f00000006028501009085010090850000908500009000010402c48993fe0f03000"
                "47f000000000000"
                "7f0000000000007f000000060b0084703dba85cccc1c84703dba85cccc7400850ad7cc84703dba86ecd17484703dba86b89e018590c"
                "26586b89e2d86a4"
                "702185cccc7485703dba84cccc1c86a4702184cccccc8590c2650084703dba00010502c48993fe0f0300047f0000000000007f00000"
                "00000007f000000"
                "060b0084703dba85cccc1c84703dba85cccc7400850ad7cc84703dba86ecd17484703dba86b89e018590c26586b89e2d86a4702185c"
                "ccc7485703dba84"
                "cccc1c86a4702184cccccc8590c2650084703dba00010602ff8180f80f0300047f0000000000007f0000000000007f000000060b008"
                "4703dba85cccc1c"
                "84703dba85cccc7400850ad7cc84703dba86ecd17484703dba86b89e018590c26586b89e2d86a4702185cccc7485703dba84cccc1c8"
                "6a4702184cccccc"
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

void sindy::triggeredImportJson()
{
    // QString filter = "json文件(*.json)";
    // QString fileName = QFileDialog::getOpenFileName(nullptr, "请选择json文件", QDir::currentPath(), filter);
    // if (fileName.isEmpty())
    //    return;

    // std::string path = fileName.toStdString();
    std::string path = "large_segmentation.json";

    std::string contents;
    sindy::readContents(path, contents);

    size_t cursor = 0;
    size_t length = contents.size();
    if (length < 10)
        return;

    GeArchive().doc()->reset();

    while (cursor < length)
    {
        int64_t index = contents.find('\n', cursor);
        if (index < 0)
            index = length;

        auto count = index - cursor;
        auto line  = contents.substr(cursor, count);
        cursor     = index + 1;

        importJson(line);
    }
}

void sindy::importJson(std::string const& line)
{
    iguana::jvalue value;
    iguana::parse(value, line);

    // 多边形
    auto&& arr  = value.to_array();
    auto   size = arr.size();
    if (size < 2)
        return;

    auto&& objPolygon = arr[0].to_object();
    int    type1      = 0;
    if (!sindy::findInt(objPolygon, "type", type1))
        return;

    auto type = (IGeGraphic::ObjectType)type1;
    if (type != IGeGraphic::ObjectType::eGePolygonType)
        return;

    auto iter = objPolygon.find("index");
    if (iter == objPolygon.end())
        return;

    auto&& edges    = iter->second.to_array();
    auto   edgeSize = edges.size();

    size_t           count = 0;
    std::vector<int> numEdges;
    for (auto i = 0; i < edgeSize; ++i)
    {
        int val = edges[i].to_int();
        count += val;
        numEdges.emplace_back(val);
    }

    if (count != size - 1)
        return;

    auto pScene = GeArchive().scene();
    if (!pScene)
        return;

    auto polygon = new GePolygon(numEdges);

    // 边
    size_t index = 1;
    for (auto i = 0; i < edgeSize; ++i)
    {
        int num = numEdges[i];
        for (auto j = 0; j < num; ++j, ++index)
        {
            auto&& element = arr[index].to_object();

            int type1 = 0;
            if (!sindy::findInt(element, "type", type1))
                return;

            auto strObject = sindy::stringify(arr[index]);

            type = (IGeGraphic::ObjectType)type1;
            switch (type)
            {
                case IGeGraphic::ObjectType::eGeSegmentType:
                {
                    auto seg = std::make_shared<sindy::Segment>();
                    iguana::from_json(*seg, strObject);

                    polygon->addEdge(seg);
                    break;
                }
                case IGeGraphic::ObjectType::eGeArcType:
                {
                    auto arc = std::make_shared<sindy::Arc>();
                    iguana::from_json(*arc, strObject);
                    polygon->addEdge(arc);
                    break;
                }
                default:
                    return;
            }
        }
    }

    pScene->addItem(polygon);
}
