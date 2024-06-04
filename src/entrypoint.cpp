/**
 * Vetices: Point
 * Facets: Triangle
 * Attributes: PointAttribute, FacetAttribute
 * Halfedges:
*/
#include "helpers.h"
#include <ultimaille/all.h>


using namespace UM;

std::string resPath = "res/";
std::string meshPath = "catorus.geogram";

int main() {
    // Create a directory to save files
    createDirectory("res");

    // Get the path to the assets
    std::string path = getAssetPath();


    // Declare a mesh with triangle surface
    Triangles m;
    Quads q;
    // Loading mesh into m
    read_by_extension(path + meshPath, m);

    PointAttribute<double> pa(m.points);
    FacetAttribute<int> fa(m);
    vec3 firstVertice, secondVertice, newVertice;
    q.points.create_points(m.nverts() + m.nfacets() * 3);
    q.create_facets(m.nfacets());
    for (Verti v : m.vertices()) {
        pa[v] = 1;
    }

    for (auto f : m.iter_facets() ) {
        fa[f] = 1;
        for (int i = 0; i < 3; i++) {
            pa[f.vertex(i)] += 1;
            firstVertice = f.vertex(i).pos();
            secondVertice = f.vertex((i+1) % 3).pos();
            newVertice = (firstVertice + secondVertice) / 2;
        }
    }



    // Save mesh with previously created attribute
    write_by_extension(resPath + meshPath, m, {{{"number_time_used", pa.ptr}}, {{"treated", fa.ptr}}, {}});

    return 0;
}