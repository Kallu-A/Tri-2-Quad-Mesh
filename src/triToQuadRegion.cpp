#include <ultimaille/all.h>
#include "region.cpp"

using namespace UM;

// Function to calculate the number of region to create for that each region will have a approx percentage of the total number of facets
int calculateNumberRegion(Triangles &triangle, double percentage) {
    return triangle.nfacets()  / (triangle.nfacets() * percentage);
}

// Algorithm to convert a triangle mesh to a quad mesh
void process(Triangles &triangle, Quads &quad, FacetAttribute<int> &fa, int numberRegion) {
    std::vector<Region> regions;
    std::vector<int> regionFacet;
    int randomFacet;
    int maxFacet = triangle.nfacets();
    if (numberRegion > maxFacet) {
        std::cout << "Number of region is too high, not enough Facet" << std::endl;
        exit(1);
    }
    for (int i = 0; i < numberRegion; i++) {
        do {
            randomFacet = rand() % maxFacet;
        } while (std::find(regionFacet.begin(), regionFacet.end(), randomFacet) != regionFacet.end());
        
        regionFacet.push_back(randomFacet);
        fa[randomFacet] = i;

        regions.push_back(Region(randomFacet, triangle, i));
    }

    bool somethingChange = false;
    do {
        somethingChange = false;
        for (auto &region : regions) {
            std::vector<int> adjacentFacet = region.getAdjacentFacet();
            for (auto f : adjacentFacet) {
                if (fa[f] != 0) {
                    continue;
                }
                fa[f] = region.getIdGroup();
                region.addFacet(f);
                somethingChange = true;
            }
        }
    } while (somethingChange);
    
}
