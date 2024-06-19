/**
 * Vetices: Point
 * Facets: Triangle
 * Attributes: PointAttribute, FacetAttribute
 * Halfedges:
*/
#include "utils/helpers.h"
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

bool gifmode = false;

int main(int argc, char* argv[]) {
    Parameters params;
    params.add(Parameters::Type::String, "path", "").description("Path to the mest");
    params.add(Parameters::Type::Bool, "gif", "false").description("Create a gif of the process");
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
        isNumberRegion = true;
        if (percentage > 1 || percentage < 0) {
            std::cout << "Error: percentage must be between 0 and 1" << std::endl;
            return 1;
        }
    }
    if (std::string(params["gif"]) == "true") {
        gifmode = true;
    }

    // Create a directory to save files
    createDirectory("result");


    if (!isInputPath) 
        inputPath = getAssetPath();
    else 
        inputPath = "";

    //Start timer 
    auto start = std::chrono::high_resolution_clock::now();

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

    if (!isNumberRegion) {
        numberRegion = calculateNumberRegion(triangle, percentage);
        numberRegion = -1;
    }

    triangle.connect();

    std::cout << std::endl << "Initial file: " << inputPath + meshPath << std::endl
        << "vertices: " << triangle.nverts()
        << " facets: " << triangle.nfacets() << std::endl;


    size_t last_slash = meshPath.find_last_of('/');
    if (last_slash != std::string::npos) {
        meshPath = meshPath.substr(last_slash + 1);
    }

    FacetAttribute<int> fa(triangle);
    PointAttribute<int> pa(triangle.points);
    CornerAttribute<int> ea(triangle, -1);
    FacetAttribute<int> faQuad(quad);
    for (int i = 0; i < triangle.ncorners(); i++) {
        ea[i] = -1;
    }

    process(triangle, quad, fa, pa, ea, numberRegion, faQuad, gifmode);
    if (gifmode) {
        write_by_extension("result/region/09999.geogram", triangle, {{{"border_group", pa.ptr}}, {{"group_number", fa.ptr}}, {{"edge_group", ea.ptr}}});

    }   

    // --- FILE SAVING ---


    std::filesystem::path fs_path(meshPath);
    std::string base_name = fs_path.stem().string();
    std::string extension = fs_path.extension().string();
    bool isGeogram = true;
    if (isGeogram)
        extension = ".geogram";
    std::string pathTri = (fs_path.parent_path() / std::string(base_name + "_tri" + extension)).string();
    std::string pathQuad = (fs_path.parent_path() / std::string(base_name + "_quad" + extension)).string();
    // Save mesh with previously created attribute

    write_by_extension(resPath + "/" + pathTri, triangle, {{{"border_group", pa.ptr}}, {{"group_number", fa.ptr}}, {{"edge_group", ea.ptr}}});
    write_by_extension(resPath + "/" + pathQuad, quad, {{}, {{"intial_region", faQuad.ptr}}, {}});
    std::cout << std::endl << "Result file: " << resPath + "/" +  pathTri << std::endl;
    std::cout << std::endl << "Result file: " << resPath + "/" +  pathQuad << std::endl;

    std::cout << "Quad Mesh data: number of vertices: " << quad.nverts() << " number of facets: " << quad.nfacets() << std::endl << std::endl;
    //std::cout << "Number of vertices: " << q.nverts();
    //std::cout << "Number of facets: " << q.nfacets() << std::endl;

    //End timer
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Time taken: " << duration.count() << " miliseconds" << std::endl;

    return 0;
}

