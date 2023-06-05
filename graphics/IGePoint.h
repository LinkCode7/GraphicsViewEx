#ifndef I_GE_POINT_H
#define I_GE_POINT_H

#include "IGeGraphic.h"

class IGePoint : public IGeGraphic
{
public:
    IGePoint(QPointF const &pt, IGeGraphic::ObjectType type);

    QPointF point() const { return _pt; }
    void    point(QPointF const &pt) { _pt = pt; }

protected:
    QPointF _pt = {};
};

#endif // !I_GE_POINT_H
