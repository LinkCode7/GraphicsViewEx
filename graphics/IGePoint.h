#ifndef I_GE_POINT_H
#define I_GE_POINT_H

#include "IGePointSet.h"

class IGePoint : public IGePointSet
{
public:
    IGePoint(QPointF const &pt, IGeGraphic::ObjectType type);

    inline QPointF point() const
    {
        assert(_points.size() == 1);
        return _points[0];
    }
    inline void point(QPointF const &pt)
    {
        assert(_points.size() == 1);
        _points[0] = pt;
    }
};

#endif // !I_GE_POINT_H
