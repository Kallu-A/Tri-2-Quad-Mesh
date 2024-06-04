/**
 * This example shows how to create attributes of different type, fill them
 * save them into a mesh file and reload them from a mesh file
*/
#include "helpers.h"
#include <ultimaille/all.h>


using namespace UM;

int main() {

    // --- LOAD ---

    // Get path of current executable
    std::string path = getAssetPath();

    // Declare a mesh with triangle surface
    Triangles m;
    // Loading catorus.geogram into m
    read_by_extension(path + "catorus.geogram", m);

    // --- POINT ATTR ---

    // Create a point attribute
    PointAttribute<double> pa(m.points);

    for (auto v : m.iter_vertices()) {
        // Compute manhattan distance (l1-norm) between origin and vertex
        auto d = vec3(0,0,0) - v.pos();
        pa[v] = std::abs(d.x) + std::abs(d.y) + std::abs(d.z);
    }

    // --- SAVE POINT ---

    // Save mesh with previously created attribute
    write_by_extension("res/catorus_manhattan.geogram", m, {{{"pa", pa.ptr}}, {}, {}});

    // --- FACET ATTR ---

    // Create a facet attribute
    FacetAttribute<int> fa(m);

    // For all facets in mesh, associate a random int value between [0-99]
    for (auto f : m.iter_facets())
        fa[f] = rand() % 100;

    // --- SAVE FACET ---

    // Save mesh with previously created attribute
    write_by_extension("res/catorus_fa.geogram", m, {{}, {{"fa", fa.ptr}}, {}});

    // --- CORNER ATTR ---

    // TODO

    // --- SAVE CORNER ---

    // TODO

    // --- SAVE ALL ATTRIBUTES ---

    // Save mesh with all previously created attributes
    write_by_extension("res/catorus_attr.geogram", m, {{{"pa", pa.ptr}}, {{"fa", fa.ptr}}, {}});

    // --- READ ATTRIBUTES ---

    // Load mesh and read attributes
    Triangles m2;
    SurfaceAttributes attributes = read_by_extension("res/catorus_attr.geogram", m2);
    // Load "pa" attribute
    PointAttribute<double> pa2("pa", attributes, m2);
    // Load "fa" attribute
    FacetAttribute<int> fa2("fa", attributes, m2);
    // Load "ca" attribute
    // TODO

    std::cout 
        << "PointAttribute size: " << pa2.ptr.get()->data.size() 
        << ", FacetAttribute size: " << fa2.ptr.get()->data.size() 
        << std::endl;

    // --- END ---

    return 0;
}