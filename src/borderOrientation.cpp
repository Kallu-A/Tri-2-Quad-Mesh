#include <ultimaille/all.h>
#include <set>
#include <string>
#include <map>

#include "intersectData.cpp"

using namespace UM;

class borderOrientation {

    private:
        std::unordered_map<std::string, std::set<int>>  mapBorder;
        std::unordered_map<std::string, int>  intersectMapBorder;

        // Function to generate a key name for the map of border
    // The advantage of this function is that it will always generate the same key for the same pair of vertices
    static std::string generateKeyName(int v0, int v1) {
        if (v0 > v1)
            std::swap(v0, v1);
        return std::to_string(v0) + "-" + std::to_string(v1);
    }


    // Helper function to generate a key name for the intersection map
    static std::string generatorKeyNameList(std::vector<int> list) {
        std::string key = "";
        std::sort(list.begin(), list.end());
        for (int i = 0; i < list.size(); i++) {
            key += std::to_string(list[i]);
            if (i != list.size() - 1) {
                key += "-";
            }
        }

        return key;
    }


    public:
    borderOrientation() {}

    // Return all the key of the map that contain the group in parameter
    // Allow to easily retrieve all border of a region 
    std::vector<std::string> getAllKeyFromGroup(int group) const {
        std::vector<std::string> keys;
        for (auto key : mapBorder) {
            if (getAllKeyContainNumbers(key.first, group)) {
                keys.push_back(key.first);
            }
        }
        return keys;
    }  

    std::vector<std::string> getAllKeyIntersectFromGroup(int group) const {
        std::vector<std::string> keys;

        for (auto key : intersectMapBorder) {

            if (isElementInString(key.first, group)) {
                keys.push_back(key.first);
            }
        }
        return keys;
    }  
    
    // Fill the map with the vertices intersection of the border
    void calculateIntersectionBorder(Triangles &triangle, Quads &quad, FacetAttribute<int> &fa, PointAttribute<int> &pa, CornerAttribute<int> &ca, std::vector<Region> &regions, bool gifmode = false) {
        
        for (auto &region : regions) {
            std::cout << "Get border vertice..." << std::endl;

            std::vector<int> borderVertices = region.getBorderVertice(ca);

            for (int i = 0; i < borderVertices.size(); i++) {
                std::set<int> regionMeet = std::set<int>();

                regionMeet.insert(region.getIdGroup());
                auto v = Surface::Vertex(triangle, borderVertices[i]);
                for (auto h : v.iter_halfedges()) {
                    auto f = Surface::Halfedge(triangle, h);
                    regionMeet.insert(fa[f.facet()]);
                    if (f.opposite() == -1) {
                        regionMeet.insert(borderOut);
                    }
                }
                if (regionMeet.size() > 2) {
                    std::vector<int> listRegionMeet = std::vector<int>();
                    listRegionMeet = std::vector<int>(regionMeet.begin(), regionMeet.end());
                    std::cout << "Generate key name..." << std::endl;

                    std::string key = generatorKeyNameList(listRegionMeet);
                    intersectMapBorder[key] = borderVertices[i];
                    std::cout << "Fill intersect..." << std::endl;

                    fillIntersect(key, borderVertices[i], triangle, fa);
                }
            }
        }

        //iter on all key of the inter map to display it 
        for (auto key : intersectMapBorder) {
            pa[key.second] = 100;
        }

    }

    // Function to classify border in region
    void calculateBorder(Triangles &triangle, Quads &quad, FacetAttribute<int> &fa, PointAttribute<int> &pa, CornerAttribute<int> &ca, std::vector<Region> &regions, bool gifmode = false) {
    
        for (auto &region : regions) {
            int idGroup = region.getIdGroup();
            std::cout << "get id group " << std::endl;
            std::vector<int> borderHalfEdge = region.getBorderHalfEdge(ca);
            std::cout << "get border halfedge " << std::endl;

            for (auto fId : borderHalfEdge) {
                auto f = Surface::Halfedge(triangle, fId);
                 std::string key;
                if (f.opposite() == -1) 
                   key = generateKeyName(idGroup, borderOut);
                else
                    key = generateKeyName(idGroup, fa[f.opposite().facet()]);
                std::cout << "gen keyframe " << std::endl;
                
                auto key_it = mapBorder.emplace(key, std::set<int>()).first;
                key_it->second.insert(f.from());
                key_it->second.insert(f.to());
                //pa[f.from()] = idGroup + fa[f.opposite().facet()];
                //pa[f.to()] = idGroup + fa[f.opposite().facet()];
            }
        }

    }

    // Return the map of the intersection border
    std::unordered_map<std::string, int> getMapIntersectBorder() {
        return intersectMapBorder;
    }

    // Return the map of the border
    std::unordered_map<std::string, std::set<int>> getMapBorder() {
        return mapBorder;
    }
};