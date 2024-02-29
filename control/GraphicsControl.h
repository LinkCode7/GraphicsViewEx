#ifndef GRAPHICS_CONTROL_H
#define GRAPHICS_CONTROL_H
#include <memory>
#include <string>

#include "qnamespace.h"

class GraphicsView;
class QKeyEvent;

namespace sindy
{
void viewKeyDown(QKeyEvent* event, GraphicsView* view);

void onCommandline(std::string const& strCommand);

void onImportFromJson();
void _importFromJson(std::string const& line);

void onOpenDocument();
void openDocument(std::string const& filename, GraphicsView* view = nullptr);

void onSaveDocument();
void saveDocument(GraphicsView* view);

void createSelectedBox();

void onCreatePolyline();
void createPolyline(GraphicsView* view);
void createPolygon(GraphicsView* view);

void deleteSelectedItems();

// 布尔运算：交、并、差
void booleanIntersection();
void booleanUnion();
void booleanDifference();

} // namespace sindy

#endif // GRAPHICS_CONTROL_H
