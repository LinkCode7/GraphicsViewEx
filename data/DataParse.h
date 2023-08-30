#ifndef DATA_SAVE_H
#define DATA_SAVE_H
#include <string>

#include "RuntimeFlag.h"
#include "SindyKiwi.h"

class GraphicsView;

class ParseGraphicsData : public VisitGraphics
{
public:
    void reset(GraphicsView* pView, std::string const& strHex);
    void fromFile(GraphicsView* pView, std::string const& filename);

    void decode(GraphicsView* pView);

public:
    void visit(IGeGraphic* pItem) override;
    void visit(IGePoint* pItem) override;
    void visit(IGePointSet* pItem) override;

    void visit(GeBox* pItem) override;
    void visit(GeSymbolPoint* pItem) override;
    void visit(GePolyline* pItem) override;
    void visit(GeSquarePoints* pItem) override;
    void visit(GePolygon* pItem) override;

private:
    IGeGraphic* createObject();

    void decodeNode(GraphicsView* pView);

private:
    SindyKiwi            _kiwi;
    sindyk::GraphicNode* _node;
};

#endif // !DATA_SAVE_H
