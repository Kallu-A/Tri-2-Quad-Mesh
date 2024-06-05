#include <ultimaille/all.h>
#include "region.cpp"

using namespace UM;

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

    for (int i = 0; i < 2; i++) {
        for (auto &region : regions) {
            std::vector<int> adjacentFacet = region.getAdjacentFacet();
            for (auto f : adjacentFacet) {
                fa[f] = region.getIdGroup();
                region.addFacet(f);
            }
        }
    }
}
