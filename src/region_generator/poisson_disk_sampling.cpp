#include <ultimaille/all.h>
#include  "../utils/helpers.h"
#include "../region.h"

int gifcounter = 0;

void generate_region(Triangles &triangle, FacetAttribute<int> &fa, PointAttribute<int> &pa, std::vector<Region> &regions,  bool gifmode = false)  {
    int nRegion = triangle.nfacets() /8;
    int nDistance = (triangle.nfacets() / nRegion);
    int k = 1000; //Number of try before we reduce the distance
    int kCounter = 0;
    std::vector<int> adjacentFacet;
    std::cout << "Number of region to create: " << nRegion << std::endl;
    std::cout <<"Initial Distance:  " << nDistance << std::endl;
    for (int i = 0;  i < nRegion;  i++) {
        int distance =  1;
        int startingPoint = rand() % triangle.nfacets();
        fa[startingPoint] = i;
        auto region = Region(startingPoint, triangle, i);
        bool isPossible = true;
        do {
            adjacentFacet = region.getAdjacentFacet();
                for (auto f : adjacentFacet) {
                    if (fa[f] != 0) {
                        isPossible = false;
                        break;
                    }
                    region.addFacet(f);
                }
            distance++;
        }  while (distance < nDistance);
      

        if (isPossible) {
            kCounter = 0;
            regions.push_back(Region(startingPoint, triangle, i));
            

            if (gifmode) {
                //save the mesh
                createDirectory("result");
                createDirectory("result/region");
                std::string path = "result/region/" + convertToNumberId(gifcounter) + ".geogram";
                write_by_extension(path, triangle, {{{"border_inters", pa.ptr}}, {{"group_number", fa.ptr}}, {}});

                gifcounter++;
            }

        }
        else {
            fa[startingPoint] = 0;
            i--;
            kCounter++;
            if (kCounter == k) {
                std::cout << "Reducing distance" << std::endl;
                nDistance--;
                kCounter = 0;
            }
            if (nDistance <= 1) {
                std::cout << "Not enough space to create a region" << std::endl;
                break;
            }
        }
    }


}

