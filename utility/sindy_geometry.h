#ifndef SINDY_GEOMETRY_H
#define SINDY_GEOMETRY_H
#include <memory>
#include <vector>

#include "sindy_point2d.h"

namespace sindy
{
class Geometry
{
public:
    virtual ~Geometry() {}

public:
    virtual Point2d begin() const  = 0;
    virtual double  length() const = 0;

    virtual std::vector<Point2d> segment(double unitLength) const = 0;
};
using GeometrySp = std::shared_ptr<Geometry>;

} // namespace sindy

#endif // !SINDY_GEOMETRY_H
