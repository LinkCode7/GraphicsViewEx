#ifndef GRAPHICS_CONTROL_H
#define GRAPHICS_CONTROL_H
#include <memory>

#include "qnamespace.h"

class GraphicsView;

namespace sindy
{
void viewKeyDown(Qt::Key key, GraphicsView* view);

}

#endif  // GRAPHICS_CONTROL_H
