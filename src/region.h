#ifndef _REGION_H_
#define _REGION_H_


#include <ultimaille/all.h>
#include <set>

using namespace UM;

int borderOut = 0; 

/**
 * @brief Class to represent a region of the mesh
 * region is a vector of facet id
 * triangle is the mesh
 * idGroup is the id of the group
*/

class Region {
    public:
        Region(int beginning, Triangles &triangle, int idGroup) : region{beginning}, triangle{triangle}, idGroup{idGroup} {}

    std::vector<int> getAllVerticeRegion() const {
        std::set<int> numbers;
        for (int i = 0; i < region.size(); i++) {
            auto f = Surface::Facet(triangle, region[i]);
            for (int j = 0; j < 3; j++) {
                numbers.insert(f.vertex(j));
            }
        }
        return std::vector<int>(numbers.begin(), numbers.end());
    }

    std::vector<int> getAdjacentFacet() const {
        std::vector<int> adjacentFacet;
        

        for (int i = 0; i < region.size(); i++) {
            //parcours les 3 voisin de chaque facet et ajouté au vecteur si il n'est pas déjà dans la region ou dans les voisins de la region
            auto f = Surface::Facet(triangle, region[i]);

            auto v0 = f.halfedge(0).opposite().facet();
            auto v1 = f.halfedge(1).opposite().facet();
            auto v2 = f.halfedge(2).opposite().facet();

            if (f.halfedge(0).opposite() != -1)
                if (!isElementInVector(region, v0) && !isElementInVector(adjacentFacet, v0))
                    adjacentFacet.push_back(v0);
            if (f.halfedge(1).opposite() != -1)
                if (!isElementInVector(region, v1) && !isElementInVector(adjacentFacet, v1))
                    adjacentFacet.push_back(v1);
            if (f.halfedge(2).opposite() != -1)
                if (!isElementInVector(region, v2) && !isElementInVector(adjacentFacet, v2))
                    adjacentFacet.push_back(v2);
    
        }
        return adjacentFacet;
    }

    std::vector<int> getRegion() const {
        return region;
    }

    // return a vector of vertices that are on the border of the region
    std::vector<int> getBorderVertice(CornerAttribute<int> &ca) const {
        std::vector<int> border;

        for (int i = 0; i < region.size(); i++) {
            //parcours les 3 voisin de chaque facet et ajouté au vecteur si il n'est pas déjà dans la region ou dans les voisins de la region
            auto f = Surface::Facet(triangle, region[i]);

            auto v0 = f.halfedge(0);
            auto v1 = f.halfedge(1);
            auto v2 = f.halfedge(2);

            if (!isElementInVector(region, v0.opposite().facet()) || v0.opposite() == -1) {
                border.push_back(v0.from());
                border.push_back(v0.to());
                ca[v0] = idGroup;
            }
            if (!isElementInVector(region, v1.opposite().facet()) || v1.opposite() == -1) {
                border.push_back(v1.from());
                border.push_back(v1.to());
                ca[v1] = idGroup;
            }
            if (!isElementInVector(region, v2.opposite().facet()) || v2.opposite() == -1) {
                border.push_back(v2.from());
                border.push_back(v2.to());
                ca[v2] = idGroup;
            }
        }
        return border;
    }

    std::vector<int> getBorderHalfEdge(CornerAttribute<int> &ca) const {
        std::vector<int> border;

        for (int i = 0; i < region.size(); i++) {
            //parcours les 3 voisin de chaque facet et ajouté au vecteur si il n'est pas déjà dans la region ou dans les voisins de la region
            auto f = Surface::Facet(triangle, region[i]);

            auto v0 = f.halfedge(0);
            auto v1 = f.halfedge(1);
            auto v2 = f.halfedge(2);

            if (!isElementInVector(region, v0.opposite().facet()) || v0.opposite() == -1) {
                border.push_back(v0);
                ca[v0] = idGroup;
            }
            if (!isElementInVector(region, v1.opposite().facet()) || v1.opposite() == -1) {
                border.push_back(v1);
                ca[v1] = idGroup;
            }
            if (!isElementInVector(region, v2.opposite().facet()) || v2.opposite() == -1) {
                border.push_back(v2);
                ca[v2] = idGroup;
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

#endif // _REGION_H_
