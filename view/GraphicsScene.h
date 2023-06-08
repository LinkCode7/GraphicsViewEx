#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include "qgraphicsscene.h"

class PropertyTable;

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(QObject* parent = nullptr);

    void setPropertyTable(PropertyTable* view) { _property = view; }

private slots:
    void selectionChanged();

private:
    PropertyTable* _property = nullptr;
};

#endif // !GRAPHICS_SCENE_H
