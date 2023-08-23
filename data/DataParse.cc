#include "DataParse.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "../graphics/GeAim.h"
#include "../graphics/GeBox.h"
#include "../graphics/GePolygon.h"
#include "../graphics/GePolyline.h"
#include "../graphics/GePolylineIndex.h"
#include "../graphics/GeSegment.h"
#include "../graphics/GeSquarePoints.h"
#include "../graphics/IGeGraphic.h"
#include "../utility/utility.h"
#include "../view/GraphicsArchive.h"
#include "../view/GraphicsView.h"
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

    GeArchive().doc()->reset();
    this->decode(pView);
}

void ParseGraphicsData::decode(GraphicsView* pView)
{
    auto doc = GeArchive().doc();
    if (_kiwi.message().create_version())
        doc->createVersion(_kiwi.message().create_version()->c_str());

    if (_kiwi.message().last_open_version())
        doc->lastOpenVersion(_kiwi.message().last_open_version()->c_str());

    if (_kiwi.message().matView())
    {
        pView->setTransform(_kiwi.toMatrix(_kiwi.message().matView()));
        // TODO:根据矩阵计算旋转角度、缩放系数，重设滚动条位置
    }

    if (_kiwi.message().background())
    {
        pView->setBgColor(*_kiwi.message().background());
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

        IGeGraphic* object = createObject();
        if (!object)
            continue;

        object->visit(this);
        scene->addItem(object);
    }
}

IGeGraphic* ParseGraphicsData::createObject()
{
    if (!_node)
        return nullptr;

    IGeGraphic* object = nullptr;

    sindyk::NodeType* objectType = _node->type();
    switch (*objectType)
    {
        case sindyk::NodeType::eBox:
            object = new GeBox({});
            break;
        case sindyk::NodeType::eChip:
            break;
        case sindyk::NodeType::eAim:
            object = new GeAim({});
            break;
        case sindyk::NodeType::ePolyline:
            object = new GePolyline({});
            break;
        case sindyk::NodeType::ePolylineIndex:
            object = new GePolylineIndex({});
            break;
        case sindyk::NodeType::eSquarePoints:
            object = new GeSquarePoints({});
            break;
        case sindyk::NodeType::eSegment:
            object = new GeSegment();
            break;
        default:
            std::cout << ">>> error type" << std::endl;
            break;
    }

    return object;
}

void ParseGraphicsData::visit(IGeGraphic* pItem)
{
    _node->set_type(static_cast<sindyk::NodeType>(pItem->objectType()));

    if (_node->id())
        pItem->id(_node->id()->c_str());

    if (_node->flags())
    {
    }

    if (_node->argb())
        pItem->setGeColor(*_node->argb());

    if (_node->mat())
    {
        QTransform trans;
        trans.translate(100, 100);
        pItem->setTransform(_kiwi.toMatrix(_node->mat()));
    }
}
void ParseGraphicsData::visit(IGePointSet* pItem)
{
    visit(static_cast<IGeGraphic*>(pItem));

    if (_node->points())
    {
        std::vector<QPointF> points;
        _kiwi.getPoints(points);
        pItem->setPoints(points);
    }
}
void ParseGraphicsData::visit(IGePoint* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}

void ParseGraphicsData::ParseGraphicsData::visit(GeBox* pItem)
{
    visit(static_cast<IGeGraphic*>(pItem));

    if (_node->rect())
        pItem->rect(_kiwi.toRect(_node->rect()));
}
void ParseGraphicsData::visit(GeAim* pItem)
{
    visit(static_cast<IGePoint*>(pItem));
}
void ParseGraphicsData::visit(GePolyline* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}
void ParseGraphicsData::visit(GePolylineIndex* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}

void ParseGraphicsData::visit(GeSquarePoints* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}

void ParseGraphicsData::visit(GeSegment* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}

void ParseGraphicsData::visit(GePolygon* pItem)
{
}
