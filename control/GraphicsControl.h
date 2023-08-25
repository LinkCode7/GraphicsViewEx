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

void onImportFromJson();
void _importFromJson(std::string const& line);

void onOpenDocument();
void openDocument(std::string const& filename, GraphicsView* view = nullptr);

void onSaveDocument();
void saveDocument(GraphicsView* view);

void createSelectedBox();

void onCreatePolyline();
void createPolyline(GraphicsView* view);

void deleteSelectedItems();

} // namespace sindy

#endif // GRAPHICS_CONTROL_H
