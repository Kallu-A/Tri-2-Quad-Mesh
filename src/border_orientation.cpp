#include <ultimaille/all.h>
#include <set>
#include <regex>
#include <string>

using namespace UM;

class borderOrientation {
    public:
    borderOrientation() {}

    // Return all the key of the map that contain the group in parameter
    // Allow to easily retrieve all border of a region 
    std::vector<std::string> getAllKeyFromGroup(int group) const {
        std::vector<std::string> keys;
        for (auto key : mapBorder) {
            if (isIntinStringBorderOrientationKey(key.first, group)) {
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
            std::vector<int> borderVertices = region.getBorderVertice(ca);

            for (int i = 0; i < borderVertices.size(); i++) {
                std::set<int> regionMeet = std::set<int>();
                std::vector<int> listRegionMeet = std::vector<int>();
                regionMeet.insert(region.getIdGroup());
                auto v = Surface::Vertex(triangle, borderVertices[i]);
                for (auto h : v.iter_halfedges()) {
                    auto f = Surface::Halfedge(triangle, h);
                    regionMeet.insert(fa[f.facet()]);
                }
                if (regionMeet.size() > 2) {
                    listRegionMeet = std::vector<int>(regionMeet.begin(), regionMeet.end());
                    std::string key = generatorKeyNameList(listRegionMeet);
                    intersectMapBorder[key] = borderVertices[i];
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
            std::vector<int> borderHalfEdge = region.getBorderHalfEdge(ca);
            for (auto fId : borderHalfEdge) {
                auto f = Surface::Halfedge(triangle, fId);
                std::string key = generateKeyName(idGroup, fa[f.opposite().facet()]);
                
                auto key_it = mapBorder.emplace(key, std::set<int>()).first;
                key_it->second.insert(fId);
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

    // Helper function to check if a int is in the the string id
    static bool isElementInString(const std::string& str, int element) {
        std::string num_str = std::to_string(element);

        // Check if num_str is at the beginning of the string
        if (str.substr(0, num_str.length()) == num_str) {
            if (str.length() == num_str.length() || str[num_str.length()] == '-') {
            return true;
            }
        }

        // Check if num_str is at the end of the string
        if (str.substr(str.length() - num_str.length()) == num_str) {
            if (str.length() == num_str.length() || str[str.length() - num_str.length() - 1] == '-') {
            return true;
            }
        }

        // Check if num_str is in the middle of the string
        size_t pos = str.find(num_str);
        while (pos != std::string::npos) {
            if (pos == 0 || str[pos - 1] == '-') {
            if (str.length() == pos + num_str.length() || str[pos + num_str.length()] == '-') {
                return true;
            }
            }
            pos = str.find(num_str, pos + 1);
        }

        return false;
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


    // Helper function to extract two integers from a string
    static bool isIntinStringBorderOrientationKey(const std::string& str, int borderRegion) {
        int pos1 = str.find('-');
        if (pos1 == std::string::npos) {
            throw std::invalid_argument("Invalid string format");
        }

        int num1 = std::stoi(str.substr(0, pos1));
        if (num1 == borderRegion )
            return true;
        int num2 = std::stoi(str.substr(pos1 + 1));
        return num2 == borderRegion;
    } 
};