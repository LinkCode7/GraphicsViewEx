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

void triggeredImportJson();
void importJson(std::string const& line);

} // namespace sindy

#endif // GRAPHICS_CONTROL_H
