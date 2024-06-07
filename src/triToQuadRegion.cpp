#include <ultimaille/all.h>
#include "region.cpp"


using namespace UM;


// allow to create the file to make the gif
int gifcounter = 0;

// Function to calculate the number of region to create for that each region will have a approx percentage of the total number of facets
int calculateNumberRegion(Triangles &triangle, double percentage) {
    return triangle.nfacets()  /((triangle.nfacets() * percentage));
}

void calculateVertices(Triangles &triangle, Quads &quad, FacetAttribute<int> &fa, PointAttribute<int> &pa, CornerAttribute<int> &ca, Region region, bool gifmode = false) {
   // auto middle = std::vector;
    std::vector<int> border = region.getBorder(ca);
    std::vector<int> vertices = region.getRegion();
    std::vector<int> middleVertice;
    /*for (auto f : vertices) {
        middleVertice = f;
    }*/
    for (auto f : border) {
        pa[f] = 1;
    }

}

// Algorithm to convert a triangle mesh to a quad mesh
void process(Triangles &triangle, Quads &quad, FacetAttribute<int> &fa, PointAttribute<int> &pa, CornerAttribute<int> &ca, int numberRegion, bool gifmode = false) {
    if (gifmode) {
        createDirectory("result");
        createDirectory("result/region");
        emptyDirectory("result/region");
        deleteFilesWithExtension("result/region", "geogram");
        deleteFilesWithExtension("animation/", "png");
        std::cout << "Gif mode activated result stored in result/region" << std::endl;
    }
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
            write_by_extension(path, triangle, {{{"border_group", pa.ptr}}, {{"group_number", fa.ptr}}, {}});

            gifcounter++;

        }
    } while (somethingChange);

    for (auto &region : regions) {
        calculateVertices(triangle, quad, fa, pa, ca, region, gifmode);
    }
    
}

