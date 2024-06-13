#include <ultimaille/all.h>
#include "region.cpp"
#include "borderOrientation.cpp"
#include "normalCalculator.cpp"

#include <set>
#include <map>
#include <string>

using namespace UM;


// allow to create the file to make the gif
int gifcounter = 0;

// index for the intersect part of the points
int intersectIndex = 0;

// index for the border part of the points
int borderIndex = 0;

// index for the facet when creating the quad
int indexIdFacet = 0;

// count the number of facet needed to be reverse
int countReverse = 0;

// Function to calculate the number of region to create for that each region will have a approx percentage of the total number of facets
int calculateNumberRegion(Triangles &triangle, double percentage) {
    return triangle.nfacets()  /((triangle.nfacets() * percentage));
}

void transformQuad(Triangles &triangle, Quads &quad, FacetAttribute<int> &fa, PointAttribute<int> &pa, CornerAttribute<int> &ca, 
        Region region, borderOrientation &borderOrientation, std::map<std::string, int> &idVerticeFromKey, FacetAttribute<int> &faQuad, bool gifmode = false) {
    
    std::vector<int> border = region.getBorderVertice(ca);
    UM::vec3 middleVertice = UM::vec3(0, 0, 0);
    auto middleVerticesList = region.getAllVerticeRegion();
    for (auto f : middleVerticesList) {
        auto vertice = Surface::Vertex(triangle, f);
        middleVertice += vertice.pos();
    }
    middleVertice = middleVertice * (1.0 / middleVerticesList.size());
    std::vector<std::string> keysBorder = borderOrientation.getAllKeyFromGroup(region.getIdGroup());
    
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
    for (auto key: keysBorder) {
        if(idVerticeFromKey.find(key) == idVerticeFromKey.end()) {
            idVerticeFromKey[key] = borderIndex;
            auto borderVertices = borderOrientation.getMapBorder()[key];
            middleVertice = UM::vec3(0, 0, 0);
            for (auto f : borderVertices) {
                auto vertice = Surface::Vertex(triangle, f);
                middleVertice += vertice.pos();
            }
            middleVertice = middleVertice * (1.0 / borderVertices.size());
            
            quad.points[borderIndex] = middleVertice;
            borderIndex++;
        }
    }

    // Link the vertices to create the quad
    auto facetRefForNormal = Surface::Facet(triangle, region.getRegion()[0]);
    auto refv0 = Surface::Vertex(triangle, facetRefForNormal.vertex(0)).pos();
    auto refv1 = Surface::Vertex(triangle, facetRefForNormal.vertex(1)).pos();
    auto refv2 = Surface::Vertex(triangle, facetRefForNormal.vertex(2)).pos();


    int idMiddle = region.getIdGroup() - 1;
    for (auto intersect : keysIntersect) {
        auto verticeIntersect = idVerticeFromKey[intersect];

        auto verticesBorder = getAllVerticeFromIntersectKey(intersect, region.getIdGroup());
        std::vector<std::string> border = getAllKeyContainNumbers(keysBorder, verticesBorder);
        if (border.size() != 2) {
            if (border.size() == 1) {
                std::cout << "Error: border size is not 2" << std::endl;
                std::cout << "border size: " << border.size() << std::endl;
                std::cout << "intersect: " << intersect << std::endl;
                std::cout << "region: " << region.getIdGroup() << std::endl;
                for (auto b : border) {
                    std::cout << b << std::endl;
                }
                exit(1);
            }
           
            // Try to rectify by taking out the vertice the most far away from the middle vertice
            auto middleVerticeVector = quad.points[idMiddle];
            while(border.size() != 2) {
                double maxDistance = 0;
                int idMax = 0;
                for (int i = 0; i < verticesBorder.size(); i++) {
                    auto vertice = Surface::Vertex(triangle, verticesBorder[i]);
                    double distance = (middleVerticeVector - vertice.pos()).norm();
                    if (distance > maxDistance) {
                        maxDistance = distance;
                        idMax = i;
                    }
                }
                border.erase(border.begin() + idMax);
            }

        }
        
        quad.create_facets(1);
        int idBorder1 = idVerticeFromKey[border[0]];
        int idBorder2 = idVerticeFromKey[border[1]];
        quad.vert(indexIdFacet, 0) = idMiddle;
        quad.vert(indexIdFacet, 1) = idBorder1;
        quad.vert(indexIdFacet, 2) = verticeIntersect;
        quad.vert(indexIdFacet, 3) = idBorder2;
        faQuad[indexIdFacet] = region.getIdGroup();
        Surface::Facet newFacet(quad, indexIdFacet);
        
        // Calcul the norm to verify if the quad is in the right direction
        auto v0 = Surface::Vertex(quad, idMiddle).pos();
        auto v1 = Surface::Vertex(quad, idBorder1).pos();
        auto v2 = Surface::Vertex(quad, verticeIntersect).pos();
        auto v3 = Surface::Vertex(quad, idBorder2).pos();
        auto normal = calculateNormalQuad(v0, v1, v2, v3);
        if (normal * calculateNormalTriangle(refv0, refv1, refv2) <= 0) {
            countReverse++;
            quad.vert(indexIdFacet, 1) = idBorder2;
            quad.vert(indexIdFacet, 3) = idBorder1;
            faQuad[indexIdFacet] = -region.getIdGroup();
        }

        indexIdFacet++;
    }


}


// Algorithm to convert a triangle mesh to a quad mesh
void process(Triangles &triangle, Quads &quad, FacetAttribute<int> &fa, PointAttribute<int> &pa, CornerAttribute<int> &ca, int numberRegion, FacetAttribute<int> &faQuad, bool gifmode = false ) {
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
    borderIndex = intersectIndex + borderOrientation.getMapIntersectBorder().size();
    quad.points.create_points(regions.size() + borderOrientation.getMapIntersectBorder().size() + borderOrientation.getMapBorder().size());
    for (auto &region : regions) {
        transformQuad(triangle, quad, fa, pa, ca, region, borderOrientation, idVerticeFromKey, faQuad, gifmode);
    }
    std::cout << "Number of facet reversed: " << countReverse << std::endl;
    
}

