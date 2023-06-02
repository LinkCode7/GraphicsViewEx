#ifndef DATA_PARSE_H
#define DATA_PARSE_H

#include "SaveFlags.h"
#include "SindyKiwi.h"

class GraphicsView;

class BoxGraphic;
class PointGraphic;
class PolylineGraphic;
class BasicGraphic;

namespace sindyk
{
class GraphicNode;
}

class SaveGraphicsData : public VisitGraphics
{
public:
    void test();

    void        saveAsFile(GraphicsView* pView, SaveFlags const& flag);
    std::string getHexString(GraphicsView* pView, SaveFlags const& flag);

    void encode(GraphicsView* pView, SaveFlags const& flag);

public:
    void visit(BasicGraphic* pItem) override;
    void visit(BoxGraphic* pItem) override;
    void visit(PointGraphic* pItem) override;
    void visit(PolylineGraphic* pItem) override;

private:
private:
    SindyKiwi            _kiwi;
    sindyk::GraphicNode* _node;
};

#endif // !DATA_PARSE_H
