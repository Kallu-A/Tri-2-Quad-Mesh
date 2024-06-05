/**
 * Vetices: Point
 * Facets: Triangle
 * Attributes: PointAttribute, FacetAttribute
 * Halfedges:
*/
#include "helpers.h"
#include <map>
#include <ultimaille/all.h>


using namespace UM;

std::string resPath = "result";
//std::string meshPath = "catorus.geogram";
std::string meshPath = "quarterdisk_tri.mesh";

int main() {
    // Create a directory to save files
    createDirectory("result");

    // Get the path to the assets
    std::string path = getAssetPath();


    // Save mesh with previously created attribute
    //write_by_extension(resPath + "/" +  meshPath, NULL, {{}, {}, {}});


    std::cout << "Initial file: " << path + meshPath << std::endl;
    //std::cout << "Number of vertices: " << triangle.nverts();
    //std::cout << "Number of facets: " << triangle.nfacets() << std::endl;
    std::cout << std::endl;

    std::cout << "Result file: " << resPath + "/" +  meshPath << std::endl;
    //std::cout << "Number of vertices: " << q.nverts();
    //std::cout << "Number of facets: " << q.nfacets() << std::endl;


    return 0;
}