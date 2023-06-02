#include "DataSave.h"

#include <stdio.h>

#include <fstream>

#include "../core/GraphicsView.h"
#include "../graphics/BasicGraphic.h"
#include "../graphics/BoxGraphic.h"
#include "../graphics/PointGraphic.h"
#include "../graphics/PolylineGraphic.h"
#include "../utility/utility.h"
#include "GraphicsDocument.h"
#include "qgraphicsscene.h"
#include "qmatrix.h"
#include "schema.h"

#define CURRENT_VERSION "1.1"

void SaveGraphicsData::saveAsFile(GraphicsView* pView, SaveFlags const& flag)
{
    std::string strHex = getHexString(pView, flag);

    std::fstream file;
    file.open(DOCUMENT_SAVE_FILE_NAME, std::ios::out); // bin
    file.write(strHex.c_str(), strHex.size());
    file.close();
}

std::string SaveGraphicsData::getHexString(GraphicsView* pView, SaveFlags const& flag)
{
    this->encode(pView, flag);

    kiwi::ByteBuffer bb;
    _kiwi.message().encode(bb);

    return sindy::stream2HexString(bb.data(), bb.size());
}

void SaveGraphicsData::encode(GraphicsView* pView, SaveFlags const& flag)
{
    if (!pView)
        return;

    if (flag.has(SaveFlags::eDocVersion))
    {
        _kiwi.message().set_create_version(_kiwi.fromString(Document::instance().createVersion()));
        _kiwi.message().set_last_open_version(_kiwi.fromString(CURRENT_VERSION));
    }

    if (flag.has(SaveFlags::eViewInfo))
    {
        _kiwi.message().set_mat(_kiwi.fromMatrix(QMatrix{}));
        _kiwi.message().set_background(1);
    }

    std::vector<BasicGraphic*> arrObject;
    Document::instance().getObjects(arrObject);

    auto size = arrObject.size();

    uint                             index      = 0;
    kiwi::Array<sindyk::GraphicNode> arrGraphic = _kiwi.message().set_graphics(_kiwi.pool(), size);
    for (auto const& graphic : arrObject)
    {
        _node = &arrGraphic[index++];
        _kiwi.currentNode(_node);

        graphic->visit(this);
    }
}

void SaveGraphicsData::visit(BasicGraphic* pItem)
{
    _node->set_type(static_cast<sindyk::NodeType>(pItem->objectType()));

    if (pItem->hasFlag(SaveFlags::eBasicGraphicInfo))
    {
        _node->set_id(pItem->id());
        _node->set_rgba64(pItem->getRgba64());
    }
    if (pItem->hasFlag(SaveFlags::eBasicGraphicMat))
    {
    }
}

void SaveGraphicsData::visit(BoxGraphic* pItem)
{
    visit(static_cast<BasicGraphic*>(pItem));

    if (pItem->hasFlag(SaveFlags::eBoxGraphicInfo))
    {
        _node->set_rect(_kiwi.fromRect(pItem->rect()));
    }
}
void SaveGraphicsData::visit(PointGraphic* pItem)
{
    visit(static_cast<BasicGraphic*>(pItem));

    if (pItem->hasFlag(SaveFlags::ePointGraphicInfo))
    {
        _node->set_pt(_kiwi.fromPoint(pItem->point()));
    }
}
void SaveGraphicsData::visit(PolylineGraphic* pItem)
{
    visit(static_cast<BasicGraphic*>(pItem));

    if (pItem->hasFlag(SaveFlags::ePolylineGraphic))
    {
        QVector<QPointF> points = pItem->getPoints();
        _kiwi.setPoints(points);
    }
}

void SaveGraphicsData::test()
{
    sindyk::Document _message;
    _kiwi.message().set_background(1);

    kiwi::ByteBuffer buffer;
    bool             encode_success = _kiwi.message().encode(buffer);

    sindyk::Document message2;
    kiwi::MemoryPool pool;
    bool             decode_success = message2.decode(buffer, pool);

    if (message2.background())
    {
        printf("value is %d\n", *message2.background());
    }
}