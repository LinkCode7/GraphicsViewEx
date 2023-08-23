#include "DataSave.h"

#include <stdio.h>

#include <fstream>

#include "../graphics/GeAim.h"
#include "../graphics/GeBox.h"
#include "../graphics/GePolygon.h"
#include "../graphics/GePolyline.h"
#include "../graphics/GePolylineIndex.h"
#include "../graphics/GeSegment.h"
#include "../graphics/GeSquarePoints.h"
#include "../graphics/IGeGraphic.h"
#include "../graphics/IGePoint.h"
#include "../graphics/IGePointSet.h"
#include "../utility/utility.h"
#include "../view/GraphicsArchive.h"
#include "../view/GraphicsView.h"
#include "qgraphicsscene.h"
#include "qmatrix.h"
#include "schema.h"

#define CURRENT_VERSION "1.1"

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

    auto doc = GeArchive().doc();
    if (flag.has(SaveFlags::eDocVersion))
    {
        _kiwi.message().set_create_version(_kiwi.fromString(doc->createVersion()));
        _kiwi.message().set_last_open_version(_kiwi.fromString(CURRENT_VERSION));
    }

    if (flag.has(SaveFlags::eViewInfo))
    {
        _kiwi.message().set_matView(_kiwi.fromMatrix(pView->transform()));
        _kiwi.message().set_background(pView->getBgColor().rgba64().toArgb32());
    }

    std::vector<IGeGraphic*> arrObject = doc->getObjects();
    auto                     size      = arrObject.size();

    uint                             index      = 0;
    kiwi::Array<sindyk::GraphicNode> arrGraphic = _kiwi.message().set_graphics(_kiwi.pool(), size);
    for (auto const& graphic : arrObject)
    {
        _node = &arrGraphic[index++];
        _kiwi.currentNode(_node);

        graphic->visit(this);
    }
}

void SaveGraphicsData::visit(IGeGraphic* pItem)
{
    _node->set_flags(0);
    _node->set_type(static_cast<sindyk::NodeType>(pItem->objectType()));

    if (pItem->hasFlag(SaveFlags::eIGeGraphicInfo))
    {
        _node->set_id(_kiwi.fromString(pItem->id()));
        _node->set_argb(pItem->getArgb32());
    }
    if (pItem->hasFlag(SaveFlags::eIGeGraphicMat))
    {
        _node->set_mat(_kiwi.fromMatrix(pItem->sceneTransform()));
    }
}
void SaveGraphicsData::visit(IGePointSet* pItem)
{
    visit(static_cast<IGeGraphic*>(pItem));

    if (pItem->hasFlag(SaveFlags::eIGePointSetInfo))
    {
        _kiwi.setPoints(*pItem->getPoints());
    }
}
void SaveGraphicsData::visit(IGePoint* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}

void SaveGraphicsData::visit(GeBox* pItem)
{
    visit(static_cast<IGeGraphic*>(pItem));

    if (pItem->hasFlag(SaveFlags::eGeBoxInfo))
    {
        _node->set_rect(_kiwi.fromRect(pItem->rect()));
    }
}
void SaveGraphicsData::visit(GeAim* pItem)
{
    visit(static_cast<IGePoint*>(pItem));
}
void SaveGraphicsData::visit(GePolyline* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}
void SaveGraphicsData::visit(GePolylineIndex* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}

void SaveGraphicsData::visit(GeSquarePoints* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}

void SaveGraphicsData::visit(GeSegment* pItem)
{
    visit(static_cast<IGePointSet*>(pItem));
}

void SaveGraphicsData::visit(GePolygon* pItem)
{
    visit(static_cast<IGeGraphic*>(pItem));

    if (pItem->hasFlag(SaveFlags::eIGePointSetInfo))
    {
    }
}
