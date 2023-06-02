#ifndef GRAPHICS_CONTROL_H
#define GRAPHICS_CONTROL_H
#include <memory>

#include "qnamespace.h"

class GraphicsView;
class QKeyEvent;

namespace sindy
{
void viewKeyDown(QKeyEvent* event, GraphicsView* view);

}

#endif // GRAPHICS_CONTROL_H
