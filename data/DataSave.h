#ifndef DATA_PARSE_H
#define DATA_PARSE_H

#include "RuntimeFlag.h"
#include "SindyKiwi.h"

class GraphicsView;

namespace sindyk
{
class GraphicNode;
}

class SaveGraphicsData : public VisitGraphics
{
public:
    void test();

    void        saveAsFile(GraphicsView* pView, RuntimeFlag const& flag, std::string const& filename);
    std::string getHexString(GraphicsView* pView, RuntimeFlag const& flag);

    void encode(GraphicsView* pView, RuntimeFlag const& flag);

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
    SindyKiwi            _kiwi;
    sindyk::GraphicNode* _node;
};

#endif // !DATA_PARSE_H
