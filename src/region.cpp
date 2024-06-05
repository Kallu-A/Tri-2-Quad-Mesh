#include <ultimaille/all.h>

using namespace UM;

class Region {
    public:
        Region(int beginning, Triangles &triangle) {
            region.push_back(beginning);
            triangle = triangle;
        }

    private:
        Triangles triangle;
        std::vector<int> region;

};