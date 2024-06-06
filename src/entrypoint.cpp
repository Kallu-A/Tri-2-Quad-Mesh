/**
 * Vetices: Point
 * Facets: Triangle
 * Attributes: PointAttribute, FacetAttribute
 * Halfedges:
*/
#include "helpers.h"
#include <ultimaille/all.h>
#include "triToQuadRegion.cpp"
#include <param_parser/param_parser.h>


using namespace UM;

// default save path
std::string resPath = "result";

// default mesh
std::string meshPath = "catorus.geogram";

// default path for the input 
std::string inputPath = "";
bool isInputPath = false;

//default number of region
int numberRegion = 15;
bool isNumberRegion = false;

double percentage = 0.05;



int main(int argc, char* argv[]) {
    Parameters params;
    params.add(Parameters::Type::String, "path", "").description("Path to the mest");
    params.add(Parameters::Type::Int, "n_region", "-414").description("Number of region to create");
    params.add(Parameters::Type::Double, "p_area", "-0.01").description("Percentage of the area to cover by region");
    params.init_from_args(argc, argv);
    
    std::cout << "Parameters: " << std::string(params["path"]) << " " << int(params["n_region"]) << " " << double(params["p_area"]) << std::endl;

    // param handling
    if (std::string(params["path"]) != "") {
        std::string param = params["path"];
        meshPath = param;
        isInputPath = true;
    }
    if (int(params["n_region"]) != -414) {
        numberRegion = params["n_region"];
        isNumberRegion = true;
        if (numberRegion < 0) {
            std::cout << "Error: number of region must be positive" << std::endl;
            return 1;
        }
    }
    if (!is_equal(double(params["p_area"]), -0.01)) {
        percentage = params["p_area"];
        if (percentage > 1 || percentage < 0) {
            std::cout << "Error: percentage must be between 0 and 1" << std::endl;
            return 1;
        }
    }

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

    if (!isNumberRegion)
        numberRegion = calculateNumberRegion(triangle, percentage);

    triangle.connect();

    std::cout << std::endl << "Initial file: " << inputPath + meshPath << std::endl
        << "vertices: " << triangle.nverts()
        << " facets: " << triangle.nfacets() << std::endl 
        << "area cover by region : " << percentage * 100 << " % region: " << numberRegion << std::endl;


    size_t last_slash = meshPath.find_last_of('/');
    if (last_slash != std::string::npos) {
        meshPath = meshPath.substr(last_slash + 1);
    }

    FacetAttribute<int> fa(triangle);
    PointAttribute<int> pa(triangle.points);
    process(triangle, quad, fa, pa, numberRegion);

    // Save mesh with previously created attribute
    write_by_extension(resPath + "/" +  meshPath, triangle, {{{"border_group", pa.ptr}}, {{"group_number", fa.ptr}}, {}});

    std::cout << std::endl << "Result file: " << resPath + "/" +  meshPath << std::endl;
    //std::cout << "Number of vertices: " << q.nverts();
    //std::cout << "Number of facets: " << q.nfacets() << std::endl;


    return 0;
}

