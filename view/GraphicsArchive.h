#ifndef GRAPHICS_ARCHIVE_H
#define GRAPHICS_ARCHIVE_H

#include "../data/GraphicsDocument.h"
#include "GraphicsScene.h"
#include "GraphicsView.h"
#define GeArchive GraphicsArchive::instance

class GraphicsArchive
{
    GraphicsArchive() {}

public:
    static inline GraphicsArchive& instance()
    {
        static GraphicsArchive archive;
        return archive;
    }

    Document* doc() { return &_doc; }

    GraphicsView* curView() { return _curView; }
    void          setCurView(GraphicsView* view) { _curView = view; }

    GraphicsScene* scene() { return _scene; }
    void           setScene(GraphicsScene* scene) { _scene = scene; }

private:
    Document       _doc;
    GraphicsView*  _curView = nullptr;
    GraphicsScene* _scene   = nullptr;
};

#endif // !GRAPHICS_ARCHIVE_H
