#ifndef HARD_EDGE_H
#define HARD_EDGE_H

#include <ultimaille/all.h>
#include "../region.h"
#include "normalCalculator.h"

#include <optional>
#include <cmath>

/**
 * @brief Function to detect hard edge in the mesh and fill the attributs with 1 if the edge is a hard edge
 */
void detectHardEdge(Triangles &triangle, FacetAttribute<int> &fa, PointAttribute<int> &verticesDetected, CornerAttribute<int> &edgesResult) {

        for (auto edge : triangle.iter_halfedges()) {
            auto halfedge = Surface::Halfedge(triangle, edge);
            if (edgesResult[edge] != 0 || halfedge.opposite() != -1 && edgesResult[halfedge.opposite()] != 0) {
                continue; // halfedge already treated or opposite halfedge already treated
            }
            // if opposite doesn't exist it's a hard edge but we aren't interested in it 
            if (halfedge.opposite() == -1) {
                edgesResult[halfedge] = 1;
                continue;
            }

            auto facet = Surface::Facet(triangle, halfedge.facet());
            auto facetOpposite = Surface::Facet(triangle, halfedge.opposite().facet());

            auto normalFacet = calculateNormalTriangle(facet);
            auto normalFacetOpposite = calculateNormalTriangle(facetOpposite);
            auto dotResult = dotProduct(normalFacet, normalFacetOpposite);
    
            if (dotResult < cos(30.5)) {
                edgesResult[halfedge] = 1;
                edgesResult[halfedge.opposite()] = 1;
                verticesDetected[halfedge.from()] = 1;
                verticesDetected[halfedge.to()] = 1;
            }


    }
}

/**
 * @brief Function to detect hard edge in the mesh and fill the attributs with 1 if the edge is a hard edge
 */
PointAttribute<int> detectHardEdge(Quads &quad) {
    PointAttribute<int> verticesDetected(quad.points, 0);
    CornerAttribute<int> edgesResult(quad, 0);
    for (auto edge : quad.iter_halfedges()) {
        auto halfedge = Surface::Halfedge(quad, edge);
        if (edgesResult[edge] != 0 || halfedge.opposite() != -1 && edgesResult[halfedge.opposite()] != 0) {
                continue; // halfedge already treated or opposite halfedge already treated
            }
            // if opposite doesn't exist it's a hard edge but we aren't interested in it 
            if (halfedge.opposite() == -1) {
                edgesResult[halfedge] = 1;
                continue;
            }

            auto facet = Surface::Facet(quad, halfedge.facet());
            auto facetOpposite = Surface::Facet(quad, halfedge.opposite().facet());

            auto normalFacet = calculateNormalTriangle(facet);
            auto normalFacetOpposite = calculateNormalTriangle(facetOpposite);
            auto dotResult = dotProduct(normalFacet, normalFacetOpposite);
    
            if (dotResult < cos(30.5)) {
                edgesResult[halfedge] = 1;
                edgesResult[halfedge.opposite()] = 1;
                verticesDetected[halfedge.from()] = 1;
                verticesDetected[halfedge.to()] = 1;
            }
    }
}


int isFacetEmpty(Region &region, FacetAttribute<int> &fa) {
    for (int i = 0; i < region.getRegion().size(); i++) {
        if (fa[region.getRegion()[i]] == 0 ) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Function to grow a region
    with region initial the region before being cut and region the newly created region
*/
void growthRegion(Triangles &triangle, Region &regionInitial, Region &region, FacetAttribute<int> &fa, CornerAttribute<int> &edge) {
    bool growing = false;
    fa[region.getRegion()[0]] = region.getIdGroup();
    do {

        growing = false;
        for (auto facet: region.getRegion()) {
            for (auto halfedge : Surface::Facet(triangle, facet).iter_halfedges()) {
                if (edge[halfedge] == 1) {
                    continue;
                }
                auto facetOposite = Surface::Facet(triangle, halfedge.opposite().facet());
                if (fa[facetOposite] == 0) {
                    fa[facetOposite] = region.getIdGroup();
                    region.addFacet(facetOposite);
                    growing = true;
                }
            }
        }

    } while (growing);
    
}

/**
 * @brief Function to make sure no region is cutting a hard edge allows to preserve the geometry of the mesh
 */
void cleanHardEdge(Triangles &triangle, std::vector<Region> &regions, FacetAttribute<int> &fa, PointAttribute<int> &verticesDetected) {
    CornerAttribute<int> edge(triangle, 0);
    detectHardEdge(triangle, fa, verticesDetected, edge);
    std::vector<Region> regionsToAdd;
    int regionHardEdge = 0;

    for (auto &region : regions) {
        if (!region.isHardEdgePresent(edge)) {
            continue;
        }
        for (int i = 0; i < region.getRegion().size(); i++) {
            fa[region.getRegion()[i]] = 0;
        }
        regionHardEdge++;
        int indexEmpty = -1;
        bool isFirst = true;
        std::optional<Region> regionToAssign;
        do {
            indexEmpty = isFacetEmpty(region, fa);
            if (indexEmpty != -1) {
                
                if (isFirst) {
                    Region newRegion(region.getRegion()[indexEmpty], triangle, region.getIdGroup());
                    growthRegion(triangle, region, newRegion, fa, edge);
                    isFirst = false;
                    regionToAssign.emplace(newRegion);
                    continue;
                }
                Region newRegion(region.getRegion()[indexEmpty], triangle, regions.size() + 1 + regionsToAdd.size());
                growthRegion(triangle, region, newRegion, fa, edge);
                regionsToAdd.push_back(newRegion);
                
            }
        } while (indexEmpty != -1);
        region.assign(*regionToAssign);
        

    } 
    std::cout << "Number of region cut because of a hard edge: " << regionHardEdge << std::endl;


    for (auto &region : regionsToAdd) {
        regions.push_back(region);
    }


}


#endif // HARD_EDGE_H