#include <ultimaille/all.h>

using namespace UM;

class Region {
    public:
        Region(int beginning, Triangles &triangle, int idGroup) : region{beginning}, triangle{triangle}, idGroup{idGroup} {}

    std::vector<int> getAdjacentFacet() const {
        std::vector<int> adjacentFacet;
        

        for (int i = 0; i < region.size(); i++) {
            //parcours les 3 voisin de chaque facet et ajouté au vecteur si il n'est pas déjà dans la region ou dans les voisins de la region
            auto f = Surface::Facet(triangle, region[i]);

            auto v0 = f.halfedge(0).opposite().facet();
            auto v1 = f.halfedge(1).opposite().facet();
            auto v2 = f.halfedge(2).opposite().facet();

            if (std::find(region.begin(), region.end(), v0) == region.end() && std::find(adjacentFacet.begin(), adjacentFacet.end(), v0) == adjacentFacet.end())
                adjacentFacet.push_back(v0);
            if (std::find(region.begin(), region.end(), v1) == region.end() && std::find(adjacentFacet.begin(), adjacentFacet.end(), v1) == adjacentFacet.end())
                adjacentFacet.push_back(v1);
            if (std::find(region.begin(), region.end(), v2) == region.end() && std::find(adjacentFacet.begin(), adjacentFacet.end(), v2) == adjacentFacet.end())
                adjacentFacet.push_back(v2);
    
        }
        return adjacentFacet;
    }

    std::vector<int> getRegion() const {
        return region;
    }

    // return a vector of vertices that are on the border of the region
    std::vector<int> getBorder() const {
        std::vector<int> border;

        for (int i = 0; i < region.size(); i++) {
            //parcours les 3 voisin de chaque facet et ajouté au vecteur si il n'est pas déjà dans la region ou dans les voisins de la region
            auto f = Surface::Facet(triangle, region[i]);

            auto v0 = f.halfedge(0).opposite();
            auto v1 = f.halfedge(1).opposite();
            auto v2 = f.halfedge(2).opposite();

            if (std::find(region.begin(), region.end(), v0.facet()) == region.end()) {
                border.push_back(v0.from());
                border.push_back(v0.to());
            }
            if (std::find(region.begin(), region.end(), v1.facet()) == region.end()) {
                border.push_back(v1.from());
                border.push_back(v1.to());
            }
            if (std::find(region.begin(), region.end(), v2.facet()) == region.end()) {
                border.push_back(v2.from());
                border.push_back(v2.to());
            }
        }
        return border;
    }

    int getIdGroup() const {
        return idGroup;
    }

    void addFacet(int f) {
        region.push_back(f);
    }



    private:
        Triangles &triangle;
        std::vector<int> region;
        int idGroup;

};