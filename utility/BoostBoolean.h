#ifndef BOOST_BOOLEAN_H
#define BOOST_BOOLEAN_H

#include "../graphics/GePolygon.h"

namespace boolean
{
void intersection(GePolygon* poly1, GePolygon* poly2, std::vector<GePolygon*>& result);

}

#endif // BOOST_BOOLEAN_H
