#ifndef TOPOLOGICAL_CLEANING_H
#define TOPOLOGICAL_CLEANING_H

#include <ultimaille/all.h>
#include <unordered_set>
#include "set"

void convertToDisk(Triangles &triangle, Region &region, FacetAttribute<int> &f, std::vector<Region> &regionsToAdd, int regionNumber) {
    region.clear()
}

void convertToOnlyTopogicalDisk(Triangles &triangle, FacetAttribute<int> &fa, std::vector<Region> &regions) {
    std::vector<Region> regionsToAdd;

    auto maxRegion = std::max_element(regions.begin(), regions.end(), 
        [](const Region& a, const Region& b) {
            return a.getIdGroup() < b.getIdGroup();
        });
    int regionNumber = maxRegion->getIdGroup();


    for (auto &region : regions) {
       if (!region.isDiskTopologic()) {
           convertToDisk(triangle, region, fa, regionsToAdd, regionNumber++);
       }
    }
}


#endif // TOPOLOGICAL_CLEANING_H
