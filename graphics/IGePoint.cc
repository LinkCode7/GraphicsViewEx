#include "IGePoint.h"

IGePoint::IGePoint(QPointF const &pt, IGeGraphic::ObjectType type) : IGeGraphic(type), _pt(pt)
{
}