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
std::string meshPath = "catorus.geogram";

int main() {
    // Create a directory to save files
    createDirectory("result");

    // Get the path to the assets
    std::string path = getAssetPath();


    // Declare a mesh with triangle surface
    Triangles m;
    Quads q;
    // Loading mesh into m
    read_by_extension(path + meshPath, m);
    m.connect();

    vec3 newVertice, firstVertice, secondVertice, thirdVertice;
    q.points.create_points(m.nverts() + m.nfacets() * 4);
    q.create_facets(m.nfacets());

    PointAttribute<double> pa(q.points);
    FacetAttribute<int> fa(q);

    for (auto v : m.iter_vertices()) {
        q.points[v] = v.pos();
        pa[v] = 0;
    }



    std::map<int, int> halfEdgeToEdgeMap;

    for (auto f : m.iter_facets() ) {
        //get the edge of the triangle

        firstVertice = (f.vertex(0).pos() + f.vertex(1).pos()) / 2;
        secondVertice = (f.vertex(1).pos() + f.vertex(2).pos()) / 2;
        thirdVertice = (f.vertex(2).pos() + f.vertex(0).pos()) / 2;

        newVertice = (firstVertice + secondVertice + thirdVertice) / 3;
        q.points[m.nverts() + f] = newVertice;
        pa[m.nverts() + f] = 1;
    }
    int index = m.nverts() + m.nfacets();
    for (auto e: m.iter_halfedges()) {
        if (halfEdgeToEdgeMap.find(e.opposite()) != halfEdgeToEdgeMap.end()) {
            halfEdgeToEdgeMap[e] = halfEdgeToEdgeMap[e.opposite()];
            continue;
        }
        halfEdgeToEdgeMap[e] = index;

        q.points[index] = (e.to().pos() + e.from().pos()) / 2;
        pa[index] = 2;
        index++;
    }



    // Save mesh with previously created attribute
    write_by_extension(resPath + "/" +  meshPath, q, {{{"created_method", pa.ptr}}, {{"treated", fa.ptr}}, {}});

    return 0;
}