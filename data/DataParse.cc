#include "DataParse.h"

#include <fstream>
#include <sstream>

#include "../core/GraphicsView.h"
#include "../graphics/BasicGraphic.h"
#include "../graphics/BoxGraphic.h"
#include "../graphics/PointGraphic.h"
#include "../graphics/PolylineGraphic.h"
#include "../utility/utility.h"
#include "GraphicsDocument.h"
#include "qgraphicsscene.h"
#include "schema.h"

void ParseGraphicsData::fromFile(GraphicsView* pView, std::string const& filename)
{
    std::fstream file;
    file.open(filename, std::ios::in);
    if (!file.is_open())
        return;

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    reset(pView, ss.str());
}

void ParseGraphicsData::reset(GraphicsView* pView, std::string const& strHex)
{
    size_t   size = 0;
    uint8_t* data = nullptr;
    sindy::hexString2Stream(strHex, data, size);
    if (size == 0)
        return;

    kiwi::ByteBuffer bb(data, size);
    _kiwi.message().decode(bb, _kiwi.pool());

    Document::instance().reset();
    this->decode(pView);
}

void ParseGraphicsData::decode(GraphicsView* pView)
{
    if (_kiwi.message().create_version())
        Document::instance().createVersion(_kiwi.message().create_version()->c_str());

    if (_kiwi.message().last_open_version())
        Document::instance().lastOpenVersion(_kiwi.message().last_open_version()->c_str());

    if (_kiwi.message().mat())
    {
    }

    if (_kiwi.message().background())
    {
    }

    decodeNode(pView);
}

void ParseGraphicsData::decodeNode(GraphicsView* pView)
{
    kiwi::Array<sindyk::GraphicNode>* arrNode = _kiwi.message().graphics();
    if (!arrNode)
        return;

    auto scene = pView->scene();

    auto size = arrNode->size();
    for (size_t i = 0; i < size; ++i)
    {
        _node = &(*arrNode)[i];
        _kiwi.currentNode(_node);

        BasicGraphic* object = createObject();
        if (!object)
            continue;

        object->visit(this);
        scene->addItem(object);
    }
}

BasicGraphic* ParseGraphicsData::createObject()
{
    if (!_node)
        return nullptr;

    BasicGraphic* object = nullptr;

    sindyk::NodeType* objectType = _node->type();
    switch (*objectType)
    {
        case sindyk::NodeType::eBox:
            object = new BoxGraphic({});
            break;
        case sindyk::NodeType::eChip:
            break;
        case sindyk::NodeType::ePoint:
            object = new PointGraphic({});
            break;
        case sindyk::NodeType::ePolyline:
            object = new PolylineGraphic({});
            break;
        default:
            break;
    }

    return object;
}

void ParseGraphicsData::visit(BasicGraphic* pItem)
{
    _node->set_type(static_cast<sindyk::NodeType>(pItem->objectType()));

    if (_node->id())
        pItem->id(*_node->id());

    if (_node->rgba64())
        pItem->setColor(*_node->rgba64());
}
void ParseGraphicsData::visit(BoxGraphic* pItem)
{
    visit(static_cast<BasicGraphic*>(pItem));

    if (_node->rect())
        pItem->rect(_kiwi.toRect(_node->rect()));
}
void ParseGraphicsData::visit(PointGraphic* pItem)
{
    visit(static_cast<BasicGraphic*>(pItem));

    if (pItem->hasFlag(SaveFlags::ePointGraphicInfo))
    {
        if (_node->pt())
            pItem->point(_kiwi.toPoint(_node->pt()));
    }
}
void ParseGraphicsData::visit(PolylineGraphic* pItem)
{
    visit(static_cast<BasicGraphic*>(pItem));

    if (_node->points())
    {
        QVector<QPointF> points;
        _kiwi.getPoints(points);
        pItem->setPoints(points);
    }
}