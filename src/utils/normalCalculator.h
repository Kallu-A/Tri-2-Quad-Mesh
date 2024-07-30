#ifndef NORMAL_CALCULATOR_H
#define NORMAL_CALCULATOR_H

#include <ultimaille/all.h>

using namespace UM;

// Function calcul the cross product of 2 vector
UM::vec3 crossProduct(UM::vec3 v1, UM::vec3 v2) {
    #define x 0
    #define y 1
    #define z 2
    return UM::vec3(
        v1[y] * v2[z] - v1[z] * v2[y],
        v1[z] * v2[x] - v1[x] * v2[z],
        v1[x] * v2[y] - v1[y] * v2[x]
    );
}

double dotProduct(UM::vec3 v1, UM::vec3 v2) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

// function that calculates the normal of a triangle
UM::vec3 calculateNormalTriangle(UM::vec3 v0, UM::vec3 v1, UM::vec3 v2) {
    // Calculate the normal of the triangle

    UM::vec3 normal = crossProduct(v1 - v0, v2 - v0);
    normal.normalize();
    return normal;
}

UM::vec3 calculateNormalTriangle(Surface::Facet &facet) {
    // Calculate the normal of the triangle
    UM::vec3 v0 = facet.vertex(0).pos();
    UM::vec3 v1 = facet.vertex(1).pos();
    UM::vec3 v2 = facet.vertex(2).pos();
    return calculateNormalTriangle(v0, v1, v2);
}

// function that calculates the normal of a quad
UM::vec3 calculateNormalQuad(UM::vec3 v0, UM::vec3 v1, UM::vec3 v2, UM::vec3 v3) {
    // Separate the quad in 2 triangle
    UM::vec3 normal1 = calculateNormalTriangle(v0, v1, v2);
    UM::vec3 normal2 = calculateNormalTriangle(v0, v2, v3);
    // Calculate the normal of the quad
    return (normal1 + normal2).normalized();

}

UM::vec3 calculateNormalQuad(Surface::Facet &facet) {
    // get each vec3
    UM::vec3 v0 = facet.vertex(0).pos();
    UM::vec3 v1 = facet.vertex(1).pos();
    UM::vec3 v2 = facet.vertex(2).pos();
    UM::vec3 v3 = facet.vertex(3).pos();
    return calculateNormalQuad(v0, v1, v2, v3);

}


void testNormal() {
    UM::vec3 v0 = UM::vec3(0, 0, 0);
    UM::vec3 v1 = UM::vec3(1, 0, 0);
    UM::vec3 v2 = UM::vec3(0, 1, 0);
    UM::vec3 v3 = UM::vec3(0, 1, 0);
   
    assert((calculateNormalTriangle(v0, v1, v2) - UM::vec3(0, 0, 1)).norm2() < 1e-4);
    assert((calculateNormalTriangle(v0, v2, v3) - UM::vec3(1, 0, 0)).norm2() < 1e-4);
    assert((calculateNormalTriangle(v0, v1, v3) - UM::vec3(0, 1, 0)).norm2() < 1e-4);
    assert((calculateNormalTriangle(v0, v2, v1) - UM::vec3(0, 0, -1)).norm2() < 1e-4);
    assert((calculateNormalTriangle(v1, v2, v0) - UM::vec3(0, 0, 1)).norm2() < 1e-4);
    assert((calculateNormalTriangle(v1, v0, v2) - UM::vec3(0, 0, -1)).norm2() < 1e-4);


    std::cout << "Test normal passed" << std::endl;

}

#endif