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


    // Declare a mesh with triangle surface
    Triangles triangle;
    Quads q;
    // Loading mesh into m
    read_by_extension(path + meshPath, triangle);
    triangle.connect();

    vec3 newVertice, firstVertice, secondVertice, thirdVertice;
    q.points.create_points(triangle.nverts() + triangle.nfacets() * 4);
    q.create_facets(triangle.nfacets() * 3);

    PointAttribute<double> pa(q.points);
    FacetAttribute<int> fa(q);

    for (auto v : triangle.iter_vertices()) {
        q.points[v] = v.pos();
        pa[v] = 0;
    }



    std::map<int, int> halfEdgeToEdgeMap;
    //create the point in the middle of the facet
    for (auto f : triangle.iter_facets() ) {
        //get the edge of the triangle

        firstVertice = (f.vertex(0).pos() + f.vertex(1).pos()) / 2;
        secondVertice = (f.vertex(1).pos() + f.vertex(2).pos()) / 2;
        thirdVertice = (f.vertex(2).pos() + f.vertex(0).pos()) / 2;

        newVertice = (firstVertice + secondVertice + thirdVertice) / 3;
        q.points[triangle.nverts() + f] = newVertice;
        pa[triangle.nverts() + f] = 1;
    }

    int index = triangle.nverts() + triangle.nfacets();
    //create the point in the middle of the edge
    for (auto e: triangle.iter_halfedges()) {
        if (halfEdgeToEdgeMap.find(e.opposite()) != halfEdgeToEdgeMap.end()) {
            halfEdgeToEdgeMap[e] = halfEdgeToEdgeMap[e.opposite()];
            continue;
        }
        halfEdgeToEdgeMap[e] = index;

        q.points[index] = (e.to().pos() + e.from().pos()) / 2;
        pa[index] = 2;
        index++;
    }


    index = 0;
    // Fill the facet attribute
    for (auto f : triangle.iter_facets()) {
        #define I f.vertex(0)
        #define J f.vertex(1)
        #define K f.vertex(2)
        #define H1 halfEdgeToEdgeMap[f.halfedge(0)]
        #define H2 halfEdgeToEdgeMap[f.halfedge(1)]
        #define H3 halfEdgeToEdgeMap[f.halfedge(2)]
        #define F triangle.nverts() + f
        
        // Quad 1
        q.vert(index, 0) = I;
        q.vert(index, 1) = H1;
        q.vert(index, 2) = F;
        q.vert(index, 3) = H3;
        fa[index] = 1;

        //Quad 2
        index++;
        q.vert(index, 0) = F;
        q.vert(index, 1) = H1;
        q.vert(index, 2) = J;
        q.vert(index, 3) = H2;
        fa[index] = 2;

        //Quad 3
        index++;
        q.vert(index, 0) = H3;
        q.vert(index, 1) = F;
        q.vert(index, 2) = H2;
        q.vert(index, 3) = K;
        fa[index] = 3;

        index++;
    }


    // Save mesh with previously created attribute
    write_by_extension(resPath + "/" +  meshPath, q, {{{"created_method", pa.ptr}}, {{"treated", fa.ptr}}, {}});

    return 0;
}