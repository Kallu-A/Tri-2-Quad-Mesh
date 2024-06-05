/**
 * Vetices: Point
 * Facets: Triangle
 * Attributes: PointAttribute, FacetAttribute
 * Halfedges:
*/
#include "helpers.h"
#include <ultimaille/all.h>
#include "triToQuadRegion.cpp"


using namespace UM;

// default save path
std::string resPath = "result";

// default mesh
std::string meshPath = "quarterdisk_tri.mesh";

// default path for the input 
std::string inputPath = "";
bool isInputPath = false;

//default number of region
int numberRegion = 15;

int main(int argc, char* argv[]) {
    // param handling
    if (argc > 3) {
        std::cout << "Usage: " << argv[0] << " [path_to_mesh] [number_region]" << std::endl;
        std::cout << "Usage: " << argv[0] << " [path_to_mesh]" << std::endl;
        std::cout << "Usage: " << argv[0] << " [number_region]" << std::endl;
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [path_to_mesh] [number_region]" << std::endl;
            std::cout << "Usage: " << argv[0] << " [path_to_mesh]" << std::endl;
            std::cout << "Usage: " << argv[0] << " [number_region]" << std::endl;
            return 0;
        }
        try {
            numberRegion = std::stoi(arg);
        } catch (std::exception e){
            meshPath = arg;
            isInputPath = true;
        }

    }

    std::cout << "Number of region: " << numberRegion << std::endl;

    // Create a directory to save files
    createDirectory("result");


    if (!isInputPath) 
        inputPath = getAssetPath();
    else 
        inputPath = "";



    Triangles triangle;
    Quads quad;
    if (inputPath == "")
        read_by_extension(meshPath, triangle);
    else
        read_by_extension(inputPath + meshPath, triangle);


    if (triangle.nverts() == 0) {
        std::cout << "Error: File " << inputPath + meshPath + " not found" << std::endl;
        return 1;
    }
        
    triangle.connect();

    std::cout << std::endl << "Initial file: " << inputPath + meshPath << std::endl
        << "vertices: " << triangle.nverts()
        << " facets: " << triangle.nfacets() << std::endl;


    size_t last_slash = meshPath.find_last_of('/');
    if (last_slash != std::string::npos) {
        meshPath = meshPath.substr(last_slash + 1);
    }

    process(triangle, quad, numberRegion);

    // Save mesh with previously created attribute
    write_by_extension(resPath + "/" +  meshPath, quad, {{}, {}, {}});


    std::cout << std::endl << "Result file: " << resPath + "/" +  meshPath << std::endl;
    //std::cout << "Number of vertices: " << q.nverts();
    //std::cout << "Number of facets: " << q.nfacets() << std::endl;


    return 0;
}

