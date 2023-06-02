#ifndef DATA_SAVE_H
#define DATA_SAVE_H
#include <string>

#include "SaveFlags.h"
#include "SindyKiwi.h"

class GraphicsView;

class BoxGraphic;
class PointGraphic;
class PolylineGraphic;
class BasicGraphic;

class ParseGraphicsData : public VisitGraphics
{
public:
    void reset(GraphicsView* pView, std::string const& strHex);
    void fromFile(GraphicsView* pView, std::string const& filename);

    void decode(GraphicsView* pView);

public:
    void visit(BasicGraphic* pItem) override;
    void visit(BoxGraphic* pItem) override;
    void visit(PointGraphic* pItem) override;
    void visit(PolylineGraphic* pItem) override;

private:
    BasicGraphic* createObject();

    void decodeNode(GraphicsView* pView);

private:
    SindyKiwi            _kiwi;
    sindyk::GraphicNode* _node;
};

#endif // !DATA_SAVE_H
