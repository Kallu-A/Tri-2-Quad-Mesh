#include <ultimaille/all.h>

using namespace UM;

// Function calcul the cross product of 2 vector
UM::vec3 crossProduct(UM::vec3 v1, UM::vec3 v2) {
    #define x 0
    #define y 1
    #define z 2
    return UM::vec3(v1[x] * v2[z] - v1[z] * v2[y], v1[z] * v2[x] - v1[x] * v2[z], v1[x] * v2[y] - v1[y] * v2[x]);
}

// function that calculates the normal of a triangle
UM::vec3 calculateNormalTriangle(UM::vec3 v0, UM::vec3 v1, UM::vec3 v2) {
    // Calculate the normal of the triangle

    UM::vec3 normal = crossProduct(v1 - v0, v2 - v0);
    normal.normalize();
    return normal;
}

// function that calculates the normal of a quad
UM::vec3 calculateNormalQuad(UM::vec3 v0, UM::vec3 v1, UM::vec3 v2, UM::vec3 v3) {
    // Separate the quad in 2 triangle
    UM::vec3 normal1 = calculateNormalTriangle(v0, v1, v2);
    UM::vec3 normal2 = calculateNormalTriangle(v0, v2, v3);
    // Calculate the normal of the quad
    return (normal1 + normal2).normalized();

}



