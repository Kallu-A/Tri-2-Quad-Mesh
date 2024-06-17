#include <ultimaille/all.h>

using namespace UM;

void reverseFacet(Quads &quad, int index) {
    /*quad.disconnect();
    auto f = Surface::Facet(quad, index);
    int idBorder1 = quad.vert(index, 1);
    int idBorder2 = quad.vert(index, 3);
    quad.vert(index, 1) = idBorder2;
    quad.vert(index, 3) = idBorder1;
    quad.connect();
    */
    int  v0 = quad.vert(index, 0);
    int  v1 = quad.vert(index, 1);
    int  v2 = quad.vert(index, 2);
    int  v3 = quad.vert(index, 3);

    quad.conn->active[index] = false;
    // Remove deactivated facets
    quad.compact();
    quad.conn->create_facet({v0, v3, v2, v1});
}


// Allow to make sure the orientation of each facet is correct and make sense with it's neighbors  recursive version
int arrangeOrientation(Quads &quad) {
    std::vector<int> reversedItemList = std::vector<int>();
    int reversedItem = 0;
    for (int i = 0; i < quad.nfacets(); i++) {
        auto f = Surface::Facet(quad, i);
        auto v0 = Surface::Vertex(quad, f.vertex(0)).pos();
        auto v1 = Surface::Vertex(quad, f.vertex(1)).pos();
        auto v2 = Surface::Vertex(quad, f.vertex(2)).pos();
        auto v3 = Surface::Vertex(quad, f.vertex(3)).pos();
        auto normalFacet = calculateNormalQuad(v0, v1, v2, v3);

        //Check with neighbors if more 50% > are in the opposite direction we reverse the facet
        int countReverse = 0;
        int totalCount = 0;
        for (auto h : f.iter_halfedges()) {

            auto half = Surface::Halfedge(quad, h);
            auto h2 = half.opposite();
            if (h2 != -1) {
                auto f2 = Surface::Facet(quad, h2.facet());
                auto fv0 = Surface::Vertex(quad, f2.vertex(0)).pos();
                auto fv1 = Surface::Vertex(quad, f2.vertex(1)).pos();
                auto fv2 = Surface::Vertex(quad, f2.vertex(2)).pos();
                auto fv3 = Surface::Vertex(quad, f2.vertex(3)).pos();
                auto normalFacet2 = calculateNormalQuad(fv0, fv1, fv2, fv3);
                // If the normal are in direction opposite by a lot we increment
                if ((normalFacet * normalFacet2) < 0) {
                    countReverse++;

                }
                totalCount++;
            }
        }
        // If more than 50% of the neighbors are in the opposite direction we reverse the facet
        if (countReverse - 1 > int(totalCount / 2)) {
            reversedItemList.push_back(i);
            reversedItem++;
        } 

    }
    for (auto i : reversedItemList) {
        reverseFacet(quad, i);
    }

    return reversedItem;
}


// Allow to make sure the orientation of each facet is correct and make sense with it's neighbors
void cleanDirection(Quads &quad) {
    quad.connect();
    int res = 1;
    int reversedItem = 0;
    do {
        res = arrangeOrientation(quad);
        reversedItem += res;
        std::cout << "Number of reversed item: " << res << std::endl;
    } while(res > 0);
    std::cout << "Number of reversed item 2nd phase: " << reversedItem << std::endl;

}
