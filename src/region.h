#ifndef _REGION_H_
#define _REGION_H_


#include <ultimaille/all.h>
#include <unordered_set>
#include "set"

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
        Region(int beginning, Triangles &triangle, int idGroup) : region{beginning}, triangle{triangle}, idGroup{idGroup} {
            borderLimit = std::unordered_set<int>();
            auto face = Surface::Facet(triangle, beginning);
            borderLimit.insert(face.halfedge(0));
            borderLimit.insert(face.halfedge(1));
            borderLimit.insert(face.halfedge(2));
        }

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

    bool isDiskTopologic() {
        int n_vertices = getAllVerticeRegion().size();
        int n_facet = region.size();
        int n_edges = getNumberEdge();

        return n_vertices - n_edges + n_facet == 1;

    }

    std::vector<int> getAdjacentFacet() {
        std::vector<int> adjacentFacet;
        std::unordered_set<int> borderNew;
        std::set<int> toRemove; //need to do a list of halfedge to remove and can't be removed straight away because of a special case 

        for (auto idHalf : borderLimit) {
            auto half = Surface::Halfedge(triangle, idHalf);
            if (half.opposite() == -1) {
                borderNew.insert(idHalf);
                continue;
            }
            auto f = half.opposite().facet();
            

            if (!isElementInVector(region, f) && !isElementInVector(adjacentFacet, f))
                adjacentFacet.push_back(f);
                toRemove.insert(idHalf);
                borderNew.insert(half.opposite().next());
                borderNew.insert(half.opposite().next().next());
            
    
        }
        for (auto idHalf : toRemove) {
            borderLimit.erase(idHalf);
        }
        setBorder(borderNew);
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

    int getNumberEdge() {
        std::set<int> edges;
        for (int i = 0; i < region.size(); i++) {
            auto f = Surface::Facet(triangle, region[i]);
            for (int j = 0; j < 3; j++) {
                if (edges.find(f.halfedge(j).opposite()) == edges.end())
                    edges.insert(f.halfedge(j));
            }
        }
        return edges.size();
    }

    void setBorder(std::unordered_set<int> border) {
        this->borderLimit = border;
    }

    int getIdGroup() const {
        return idGroup;
    }

    void addFacet(int f) {
        region.push_back(f);
    }

    private:
        Triangles &triangle;
        // Contains id of the facet
        std::vector<int> region;
        // Contains the halfedge id of the border
        std::unordered_set<int> borderLimit;
        int idGroup;


};

#endif // _REGION_H_
