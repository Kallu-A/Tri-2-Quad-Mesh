#include <ultimaille/all.h>
#include  "../utils/helpers.h"
#include "../region.h"

using namespace UM;

//put n region on the mesh the region are randomly put on the mesh
void generateNRegionRandom(int numberRegion, Triangles &triangle, FacetAttribute<int> &fa, PointAttribute<int> &pa, std::vector<Region> &regions, std::vector<int> &regionFacet, bool gifmode = false) {
    
    // allow to create the file to make the gif
    int gifcounter = 0;
    
    int randomFacet;
    int maxFacet = triangle.nfacets();
    
    for (int i = 1; i <= numberRegion; i++) {
        do {
            randomFacet = rand() % maxFacet;
        } while ((isElementInVector(regionFacet, randomFacet)));
        
        regionFacet.push_back(randomFacet);    

        fa[randomFacet] = i;

        regions.push_back(Region(randomFacet, triangle, i));
    }

    developRegion(triangle, regions, fa, gifmode);
}

// fill distance table with dijkstra algorithm
void fillDistance(Triangles &triangle, std::vector<int> &cumulDistanceFacet, std::vector<int> &regionDistance, Region region) {
    std::vector<int> adjacentFacet;
    int distance =  1;
    do {
        adjacentFacet = region.getAdjacentFacet();
            for (auto f : adjacentFacet) {
                if (regionDistance[f] == 0 && cumulDistanceFacet[f] != -1) {
                    cumulDistanceFacet[f] += distance;
                    regionDistance[f] = distance;
                }
                region.addFacet(f);
            }
            distance++;
    }  while (adjacentFacet.size() != 0);
   
}

// fill the mesh of new region with dijkstra to make it homogenous and make sure each region is a disk topologic
void createRegionDijkstra(Triangles &triangle, FacetAttribute<int> &fa, std::vector<Region> &regions) {
    std::vector<int> distanceAttribute(triangle.nfacets(), -1);
    std::queue<int> queue({rand() % triangle.nfacets()});
    distanceAttribute[queue.front()] = 0;
    int index = 0;
    regions.push_back(Region(queue.front(), triangle, regions.size() + 1));
    fa[queue.front()] = regions.size() + 1;
    while (index + 1 < triangle.nfacets() / 16) {
        while (true) {
            if (queue.empty()) {
                break;
            }
            int facetToProcess = queue.front();
            queue.pop();
            
            for (auto vertexRef : Surface::Facet(triangle, facetToProcess).iter_halfedges() ) {
                auto edge = Surface::Halfedge(triangle, vertexRef);
                if (edge.opposite() != -1) {
                    auto f =  edge.opposite().facet();
                    if (distanceAttribute[f] > distanceAttribute[facetToProcess] + 1 || distanceAttribute[f] == -1) {
                            distanceAttribute[f] = distanceAttribute[facetToProcess] + 1;
                            queue.push(f);

                    }
                }
            }
        }
        index++;
        auto element = std::max_element(distanceAttribute.begin(), distanceAttribute.end());
        int indice = std::distance(distanceAttribute.begin(), element);
        queue.push(indice);
        distanceAttribute[indice] = 0;
        regions.push_back(Region(indice, triangle, regions.size() + 1));
        fa[indice] = regions.size() + 1;
    }

    developRegion(triangle, regions, fa);


    
}