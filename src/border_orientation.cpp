#include <ultimaille/all.h>
#include <set>
#include <regex>
#include <string>

using namespace UM;

class borderOrientation {
    public:
    borderOrientation() {}

    // Return all the key of the map that contain the group in parameter
    std::vector<std::string> getAllKeyFromGroup(int group) const {
        std::vector<std::string> keys;
        for (auto key : mapBorder) {

            auto intExtracted = extractIntsFromString(key.first);
            if (intExtracted[0] == group || intExtracted[1] == group) {
                keys.push_back(key.first);
            }
        }
        return keys;
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
                pa[f.from()] = idGroup + fa[f.opposite().facet()];
                pa[f.to()] = idGroup + fa[f.opposite().facet()];
            }
        }

    }

    std::unordered_map<std::string, std::set<int>> getMapBorder() {
        return mapBorder;
    }

    private:
        std::unordered_map<std::string, std::set<int>>  mapBorder;

        // Function to generate a key name for the map of border
    // The advantage of this function is that it will always generate the same key for the same pair of vertices
    static std::string generateKeyName(int v0, int v1) {
        if (v0 > v1)
            std::swap(v0, v1);
        return std::to_string(v0) + "-" + std::to_string(v1);
    }

    // Helper function to extract two integers from a string
    static std::vector<int> extractIntsFromString(const std::string& str) {
        std::regex pattern("(\\d+)-(\\d+)");
        std::smatch match;
        if (std::regex_search(str, match, pattern) && match.size() == 3) {
            return {std::stoi(match[1]), std::stoi(match[2])};
        } else {
            throw std::invalid_argument("Invalid string format");
        }
    } 
};