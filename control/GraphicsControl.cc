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
            createPolyline(view);
            break;
        }
        case Qt::Key_B:
        {
            createSelectedBox();
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
                saveDocument(view);
            break;
        }
        default:
            break;
    }
}

void sindy::onImportFromJson()
{
    QString filter   = "json文件(*.json)";
    QString fileName = QFileDialog::getOpenFileName(nullptr, "请选择json文件", QDir::currentPath(), filter);
    if (fileName.isEmpty())
        return;

    std::string path = fileName.toStdString();
    // std::string path = "large_segmentation.json";

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

        _importFromJson(line);
    }
}

void sindy::_importFromJson(std::string const& line)
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
                    auto seg = std::make_shared<sindy::PolySegment>();
                    iguana::from_json(*seg, strObject);

                    polygon->addNewEdge(seg);
                    break;
                }
                case IGeGraphic::ObjectType::eGeArcType:
                {
                    auto arc = std::make_shared<sindy::PolyArc>();
                    iguana::from_json(*arc, strObject);
                    polygon->addNewEdge(arc);
                    break;
                }
                default:
                    return;
            }
        }
    }

    pScene->addItem(polygon);
}

void sindy::onOpenDocument()
{
    QString filter   = "文件(*.txt *.data)";
    QString fileName = QFileDialog::getOpenFileName(nullptr, "请选择文件", QDir::currentPath(), filter);
    if (fileName.isEmpty())
        return;

    openDocument(fileName.toStdString());
}

void sindy::openDocument(std::string const& filename, GraphicsView* view)
{
    if (!view)
        view = GeArchive().curView();

    if (!view)
        return;

    ParseGraphicsData data;
    // data.reset(view, test);
    data.fromFile(view, filename);
}

void sindy::onSaveDocument()
{
    saveDocument(GeArchive().curView());
}
void sindy::saveDocument(GraphicsView* view)
{
    if (!view)
        return;

    QFileDialog dialog;
    dialog.setNameFilter("file name(*.txt *.data)");
    dialog.exec();
    auto files = dialog.selectedFiles();
    if (files.size() != 1)
        return;

    SaveFlags flag;
    flag.full(true);

    SaveGraphicsData data;
    data.saveAsFile(view, flag, files[0].toStdString());
}

void sindy::createSelectedBox()
{
    auto pScene = GeArchive().scene();
    if (!pScene)
        return;

    auto items = pScene->selectedItems();
    for (auto const& item : items)
    {
        QRectF rectf = item->boundingRect();
        auto   mat   = item->sceneMatrix();
        rectf.translate({mat.dx(), mat.dy()});

        auto pItem = new GeBox(rectf);
        // pItem->setFlags(0);
        pScene->addItem(pItem);
    }
}

void sindy::onCreatePolyline()
{
    createPolyline(GeArchive().curView());
}
void sindy::createPolyline(GraphicsView* view)
{
    if (!view)
        return;

    auto drag = std::make_shared<PolylineDrag>();
    view->setDrag(drag);

    view->setCommand("polyline");
    view->addState(GraphicsView::eDragInit);
    // view->addState(GraphicsView::eDragging);
}