#include "DataParse.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "../graphics/GeBox.h"
#include "../graphics/GePolygon.h"
#include "../graphics/GePolyline.h"
#include "../graphics/GeSquarePoints.h"
#include "../graphics/GeSymbolPoint.h"
#include "../graphics/IGeGraphic.h"
#include "../utility/utility.h"
#include "../view/GraphicsArchive.h"
#include "../view/GraphicsView.h"
#include "qgraphicsscene.h"
#include "schema.h"

void readStream(char const* filename, uint8_t*& data, uint64_t& size)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp)
        return;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    data = new uint8_t[size];
    memset(data, 0, size);

    fseek(fp, 0, SEEK_SET);
    fread(data, sizeof(char), size, fp);
    fclose(fp);
}

void ParseGraphicsData::fromFile(GraphicsView* pView, std::string const& filename)
{
    if (filename.ends_with(".data"))
    {
        uint64_t size = 0;
        uint8_t* data = nullptr;
        readStream(filename.c_str(), data, size);

        kiwi::ByteBuffer bb(data, size);
        _kiwi.message().decode(bb, _kiwi.pool());

        GeArchive().doc()->reset();
        this->decode(pView);
        return;
    }

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

    if (_kiwi.message().maxId())
        doc->maxId(*_kiwi.message().maxId());

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

    auto size = arrNode->size();
    for (size_t i = 0; i < size; ++i)
    {
        _node = &(*arrNode)[i];
        _kiwi.currentNode(_node);

        IGeGraphic* object = createObject();
        if (!object)
            continue;

        object->addFlag(RuntimeFlag::eDocumentParsing);

        object->visit(this);
        GeArchive().addCustomItem(object);

        object->removeFlag(RuntimeFlag::eDocumentParsing);
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
        case sindyk::NodeType::eSymbolPoint:
            object = new GeSymbolPoint({});
            break;
        case sindyk::NodeType::eSegment:
        case sindyk::NodeType::eArc:
        case sindyk::NodeType::eBezier:
            break;
        case sindyk::NodeType::ePolyline:
            object = new GePolyline();
            break;
        case sindyk::NodeType::ePolygon:
            object = new GePolygon();
            break;
        case sindyk::NodeType::eSquarePoints:
            object = new GeSquarePoints();
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
        pItem->id(*_node->id());

    if (_node->name())
        pItem->name(_node->name()->c_str());

    if (_node->flags())
        pItem->saveStatus(*_node->flags());

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
void ParseGraphicsData::visit(GeSymbolPoint* pItem)
{
    visit(static_cast<IGePoint*>(pItem));

    if (_node->symbolType())
        pItem->symbolType((GraphicsSymbol::Type)*_node->symbolType());
}
void ParseGraphicsData::visit(GePolyline* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));

    if (_node->symbolType())
        pItem->symbolType((GraphicsSymbol::Type)*_node->symbolType());
}

void ParseGraphicsData::visit(GeSquarePoints* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}

void ParseGraphicsData::visit(GePolygon* pItem)
{
    visit(static_cast<IGeGraphic*>(pItem));

    if (auto pIndexes = _node->polyIndexes(); pIndexes)
    {
        auto size = pIndexes->size();

        std::vector<int> arr;
        arr.reserve(size);

        for (auto i = 0; i < size; ++i)
            arr.emplace_back((*pIndexes)[i]);

        pItem->indexes(arr);
    }

    if (auto pElements = _node->polyElements(); pElements)
    {
        auto size = pElements->size();
        for (auto i = 0; i < size; ++i)
        {
            if (auto pSeg = (*pElements)[i].segment(); pSeg)
            {
                auto segment = std::make_shared<sindy::PolySegment>();
                segment->_begin.x(*pSeg->begin()->x());
                segment->_begin.y(*pSeg->begin()->y());
                segment->_end.x(*pSeg->end()->x());
                segment->_end.y(*pSeg->end()->y());
                pItem->setEdge(segment);
            }
            else if (auto pArc = (*pElements)[i].arc(); pArc)
            {
                auto arc = std::make_shared<sindy::PolyArc>();
                arc->_center.x(*pArc->center()->x());
                arc->_center.y(*pArc->center()->y());
                arc->_radius     = *pArc->radius();
                arc->_radius2    = *pArc->radius2();
                arc->_beginAngle = *pArc->beginAngle();
                arc->_sweepAngle = *pArc->sweepAngle();
                pItem->setEdge(arc);
            }
        }
    }
}
