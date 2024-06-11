#include <ultimaille/all.h>
#include "region.cpp"
#include "border_orientation.cpp"
#include <set>


using namespace UM;


// allow to create the file to make the gif
int gifcounter = 0;

// index for the intersect part of the points
int intersectIndex = 0;

// Function to calculate the number of region to create for that each region will have a approx percentage of the total number of facets
int calculateNumberRegion(Triangles &triangle, double percentage) {
    return triangle.nfacets()  /((triangle.nfacets() * percentage));
}

void calculateVertices(Triangles &triangle, Quads &quad, FacetAttribute<int> &fa, PointAttribute<int> &pa, CornerAttribute<int> &ca, 
Region region, borderOrientation &borderOrientation, std::map<std::string, int> &idVerticeFromKey, bool gifmode = false) {
    std::vector<int> border = region.getBorderVertice(ca);
    UM::vec3 middleVertice = UM::vec3(0, 0, 0);
    for (auto f : border) {
        auto vertice = Surface::Vertex(triangle, f);
        middleVertice += vertice.pos();
    }
    middleVertice = middleVertice * (1.0 / border.size());
    std::vector<std::string> keysBorder = borderOrientation.getAllKeyFromGroup(region.getIdGroup());
    
    // TODO A OPTIMISER LA PERFORMANCE 
    std::vector<std::string> keysIntersect = borderOrientation.getAllKeyIntersectFromGroup(region.getIdGroup());
    
    int intersectNumber = borderOrientation.getMapIntersectBorder().size();
    
    quad.points[region.getIdGroup() - 1] = middleVertice;
    for (auto key: keysIntersect) {
        if(idVerticeFromKey.find(key) == idVerticeFromKey.end()) {
            idVerticeFromKey[key] = intersectIndex;
            quad.points[intersectIndex] = Surface::Vertex(triangle, borderOrientation.getMapIntersectBorder()[key]).pos();
            intersectIndex++;
        }
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
    borderOrientation borderOrientation;
    borderOrientation.calculateBorder(triangle, quad, fa, pa, ca, regions, gifmode);
    borderOrientation.calculateIntersectionBorder(triangle, quad, fa, pa, ca, regions, gifmode);
    std::map<std::string, int> idVerticeFromKey = std::map<std::string, int>();
    intersectIndex = regions.size();
    quad.points.create_points(numberRegion + borderOrientation.getMapIntersectBorder().size() + borderOrientation.getMapBorder().size());
    for (auto &region : regions) {
        calculateVertices(triangle, quad, fa, pa, ca, region, borderOrientation, idVerticeFromKey, gifmode);
    }
    
}

