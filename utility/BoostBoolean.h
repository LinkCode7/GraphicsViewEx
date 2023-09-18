#ifndef BOOST_BOOLEAN_H
#define BOOST_BOOLEAN_H

#include "../graphics/GePolygon.h"

namespace boolean
{
void intersection(GePolygon* poly1, GePolygon* poly2, std::vector<GePolygon*>& result);

void union_(GePolygon* poly1, GePolygon* poly2, std::vector<GePolygon*>& result);

void difference(GePolygon* poly1, GePolygon* poly2, std::vector<GePolygon*>& result);

} // namespace boolean

#endif // BOOST_BOOLEAN_H
