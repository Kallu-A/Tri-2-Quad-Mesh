#include <ultimaille/all.h>

using namespace UM;

class Region {
    public:
        Region(int beginning, Triangle triangle) {
            region.push_back(beginning);
            triangle = triangle;
        }

    private:
        Triangle triangle;
        std::vector<int> region;

};