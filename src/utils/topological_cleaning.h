#ifndef TOPOLOGICAL_CLEANING_H
#define TOPOLOGICAL_CLEANING_H

#include <ultimaille/all.h>
#include <unordered_set>
#include "set"

/* Recursively divide region and create a vector of region to add that are a disk topologic*/
void convertToDisk(Triangles &triangle, Region &region, FacetAttribute<int> &f, std::vector<Region> &regionsToAdd, int *regionNumber) {
    std::cout << "Separate region " << region.getIdGroup() << " with region " << *regionNumber << std::endl;
    int facetAvailable =  region.getRegion().size();
    auto regionSecond = Region( rand() % facetAvailable / 4, triangle, *regionNumber);
    for (auto &facet : region.getRegion()) {
        f[facet] = 0;
    }
    region.reset();
    region.addFacet(regionSecond.getRegion()[0] + facetAvailable / 2);
    f[regionSecond.getRegion()[0]] = regionSecond.getIdGroup();
    f[region.getRegion()[0]] = region.getIdGroup();
    regionsToAdd.push_back(regionSecond);
    
    // develop the 2 new region region and regionSecond
    std::vector<Region> regionToDevelop = {region, regionSecond};
    developRegion(triangle, regionToDevelop, f);
    if (!region.isDiskTopologic()) {
        *regionNumber = *regionNumber + 1;
        convertToDisk(triangle, region, f, regionsToAdd, regionNumber);
    }
    if (!regionSecond.isDiskTopologic()) {
        *regionNumber = *regionNumber + 1;
        convertToDisk(triangle, regionSecond, f, regionsToAdd, regionNumber);
    }


}

void convertToOnlyTopogicalDisk(Triangles &triangle, FacetAttribute<int> &fa, std::vector<Region> &regions) {
    std::vector<Region> regionsToAdd;

    auto maxRegion = std::max_element(regions.begin(), regions.end(), 
        [](const Region& a, const Region& b) {
            return a.getIdGroup() < b.getIdGroup();
        });
    int *regionNumber = new int(maxRegion->getIdGroup());


    for (auto &region : regions) {
       if (!region.isDiskTopologic()) {
            *regionNumber = *regionNumber + 1;
            convertToDisk(triangle, region, fa, regionsToAdd, regionNumber);
       }
    }
    delete regionNumber;
    for (auto &region : regionsToAdd) {
        regions.push_back(region);
    }
}


#endif // TOPOLOGICAL_CLEANING_H
