#ifndef GEOMETRICAL_CLEANING_H
#define GEOMETRICAL_CLEANING_H

#include <ultimaille/all.h>
#include "hardeEdge.h"

using namespace UM;

// use the barycenter of the neighbor to smooth the result and untangle some facet
void smoothFacet(Quads &quad) {
    PointAttribute<int> hardedge = detectHardEdge(quad);
}




#endif // GEOMETRICAL_CLEANING_H
