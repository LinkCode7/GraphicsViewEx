#include "IGePoint.h"

IGePoint::IGePoint(QPointF const &pt, IGeGraphic::ObjectType type) : IGePointSet({pt}, type)
{
}
