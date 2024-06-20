#include <ultimaille/all.h>
#include  "utils/helpers.h"
#include "region.h"

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
        if (gifmode) {
            //save the mesh
            createDirectory("result");
            createDirectory("result/region");

            std::string path = "result/region/" + convertToNumberId(gifcounter) + ".geogram";
            write_by_extension(path, triangle, {{{"border_inters", pa.ptr}}, {{"group_number", fa.ptr}}, {}});

            gifcounter++;

        }
    } while (somethingChange);
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
void createRegionDijkstra(Triangles &triangle, FacetAttribute<int> &fa, PointAttribute<int> &pa, std::vector<Region> &regions) {
    std::vector<int> cumulDistanceFacet = std::vector<int>(triangle.nfacets(), 0);
    std::vector<std::vector<int>> regionDistance;
    regions.push_back(Region(0, triangle, 1));
    //Fill the table of  distance
    int currentFacet = 0;
    int i = 0;
    do {
        std::cout << "Region " << regions.size() << std::endl;
        i++;
        cumulDistanceFacet[currentFacet] = -1;
        regionDistance.push_back(std::vector<int>(triangle.nfacets(), 0));
        regionDistance[regions.size() - 1 ][currentFacet] = -1;
        std::cout << "currentFacet " << currentFacet << std::endl;
        fillDistance(triangle, cumulDistanceFacet, regionDistance[regions.size() -  1], Region(currentFacet, triangle, regions.size() + 1));
        std::cout << "distance " << regionDistance[regions.size() - 1][currentFacet] << std::endl;
        regions.push_back(Region(currentFacet, triangle, regions.size() + 1));
        //new first facet of new region is the facet the most far away from everyone the index of the biggest value  in cumulDistanceFacet
        currentFacet =  std::distance(cumulDistanceFacet.begin(), std::max_element(cumulDistanceFacet.begin(), cumulDistanceFacet.end()));
    } while(i < 200); // while all region are not disk topologic

}