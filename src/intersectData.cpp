#include <ultimaille/all.h>
#include <set>

#include "utils/keyHelper.h"

using namespace UM;

enum class Mode {
    IN,
    OUT
};

class IntersectData {
    private: 
        std::string keyBorder;
        std::string region;
        Mode mode;

    public:
        IntersectData(std::string keyBorder, std::string region, Mode mode) : keyBorder{keyBorder}, region{region}, mode{mode} {}

        std::string getKeyBorder() const {
            return keyBorder;
        }

        std::string getRegion() const {
            return region;
        }

        Mode getMode() const {
            return mode;
        }
};

//Return the number of region concerned by the key
int getNumberOfRegionFromKey(const std::string &key) {
    int count = 1;
    for (char c : key) {
        if (c == '-') {
            count++;
        }
    }
    return count;
}


std::map<std::string, std::vector<IntersectData>> intersectDataFolder;

void fillIntersect(std::string keyIntersect, int vertices, Triangles &triangle,  FacetAttribute<int> &regionFacet) {
    auto verticesIntersect = Surface::Vertex(triangle, vertices);
    int processed = 0;
    for (auto halfedge: verticesIntersect.iter_halfedges()) {
        processed++;
        auto f = Surface::Halfedge(triangle, halfedge);
        auto region = regionFacet[f.facet()];
        auto regionOposite = regionFacet[f.opposite().facet()];
        if (f.opposite() == -1) {
            regionOposite = borderOut;
        }
        
        std::string key = generateKey(region, regionOposite);
        intersectDataFolder.emplace(keyIntersect,std::vector<IntersectData>());
    
        intersectDataFolder[keyIntersect].push_back(IntersectData(key, std::to_string(region), Mode::OUT));
        intersectDataFolder[keyIntersect].push_back(IntersectData(key, std::to_string(regionOposite), Mode::IN));
    }
    // Case where the intersect has a border with void and so the halfedge doesn't exist
    if (isElementInString(keyIntersect, borderOut) == true) {
        for (auto halfedge: verticesIntersect.iter_halfedges()) {
            if (halfedge.next().next().next().opposite() == -1 ) {
                auto f = Surface::Halfedge(triangle, halfedge);
                auto region = regionFacet[f.facet()];
                auto regionOposite = borderOut;
                
                std::string key = generateKey(region, regionOposite);
                intersectDataFolder[keyIntersect].push_back(IntersectData(key, std::to_string(region), Mode::OUT));
                intersectDataFolder[keyIntersect].push_back(IntersectData(key, std::to_string(regionOposite), Mode::IN));
            
            }
        }
    }
}

Mode getModeFromKey(std::string key, std::string border, std::string region) {
    auto data = intersectDataFolder[key];
    for (auto d : data) {
        if (d.getRegion() == region && d.getKeyBorder() == border) {
            return d.getMode();
        }
    }

    return Mode::IN;
    std::cout << "Error: no data found for key: " << key << " border: " << border << " region: " << region << "\n";
    for (auto key:  data) {
        std::cout << "Key: " << key.getKeyBorder() << " Region: " << key.getRegion() << "\n";
    }
    exit(1);
}
