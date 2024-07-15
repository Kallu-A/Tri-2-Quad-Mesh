#ifndef GEOMETRICAL_CLEANING_H
#define GEOMETRICAL_CLEANING_H

#include <ultimaille/all.h>
#include "hardEdge.h"

using namespace UM;

// use the barycenter of the neighbor to smooth the result and untangle some facet
void smoothFacet(Quads &quad) {
    std::cout << "Process to smooth the mesh with barycenter of neighbor iteration: " ;
    PointAttribute<int> hardedge(quad.points, 0);
    detectHardEdgeQuad(quad, hardedge);
    int number = 2;
  for (int i = 0; i < number; i++) {
        for (auto verticeId: quad.iter_vertices()) {
            if (hardedge[verticeId] == 1) {
                continue;
            }
            auto vertice = Surface::Vertex(quad, verticeId);
            vec3 barycenter = vec3(0, 0, 0);
            int count = 0;
            for (auto edge: vertice.iter_halfedges()) {
                auto halfedge = Surface::Halfedge(quad, edge);
                barycenter += halfedge.to().pos();
                count++;
            }
            barycenter = barycenter * (1.0 / count);
            vertice.pos() = barycenter;
        }
    }


    std::cout << number << std::endl;
}




#endif // GEOMETRICAL_CLEANING_H
