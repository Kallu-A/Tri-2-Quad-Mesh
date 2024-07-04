#ifndef HARD_EDGE_H
#define HARD_EDGE_H

#include <ultimaille/all.h>
#include "../region.h"
#include "normalCalculator.h"

#include <cmath>

/**
 * @brief Function to detect hard edge in the mesh and fill the attributs with 1 if the edge is a hard edge
 */
void detectHardEdge(Triangles &triangle, FacetAttribute<int> &fa, PointAttribute<int> &verticesDetected) {
    CornerAttribute<int> edgeDetected(triangle, 0);

        for (auto edge : triangle.iter_halfedges()) {
            auto halfedge = Surface::Halfedge(triangle, edge);
            if (edgeDetected[edge] != 0 || halfedge.opposite() != -1 && edgeDetected[halfedge.opposite()] != 0) {
                continue; // halfedge already treated or opposite halfedge already treated
            }
            // if opposite doesn't exist it's a hard edge but we aren't interested in it 
            if (halfedge.opposite() == -1) {
                edgeDetected[halfedge] = 1;
                continue;
            }

            auto facet = Surface::Facet(triangle, halfedge.facet());
            auto facetOpposite = Surface::Facet(triangle, halfedge.opposite().facet());

            auto normalFacet = calculateNormalTriangle(facet);
            auto normalFacetOpposite = calculateNormalTriangle(facetOpposite);
            auto dotResult = dotProduct(normalFacet, normalFacetOpposite);
    
            if (dotResult < cos(30)) {
                std::cout << "Hard edge detected" << std::endl;
                edgeDetected[halfedge] = 1;
                edgeDetected[halfedge.opposite()] = 1;
                verticesDetected[halfedge.from()] = 1;
                verticesDetected[halfedge.to()] = 1;
            }


    }
}

/**
 * @brief Function to make sure no region is cutting a hard edge allows to preserve the geometry of the mesh
 */
void cleanHardEdge(Triangles &triangle, std::vector<Region> &regions, FacetAttribute<int> &fa, PointAttribute<int> &verticesDetected) {
    detectHardEdge(triangle, fa, verticesDetected);
}


#endif // HARD_EDGE_H