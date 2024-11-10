#include "Objects3D/sphere.hpp"
#include <iostream>

// Declaration of utility functions
bool compareInt(int, int);
bool compareVec3(const glm::vec3 &, const glm::vec3 &, float = 0.01f);

// Test function for `genVertices`
void testGenVerticesSphere() {
    GLfloat radius = 1.0f;
    GLint slices = 4;

    Sphere sphere(radius, slices);
    std::vector<glm::vec3> vertices = sphere.genVertices();

    // Checking if the number of vertices is correct (slices + 1 for
    // latitudes and lonitudes)
    GLint expectedVertices = (slices + 1) * (slices + 1);
    assert(vertices.size() == static_cast<std::size_t>(expectedVertices));

    // Checking if the vertex coordinates are correct
    for (GLint lat = 0; lat <= slices; ++lat) {
        GLfloat theta = (lat * M_PI) / slices;
        GLfloat sinTheta = std::sin(theta);
        GLfloat cosTheta = std::cos(theta);

        for (GLint lon = 0; lon <= slices; ++lon) {
            GLfloat phi = (lon * 2 * M_PI) / slices;
            GLfloat sinPhi = std::sin(phi);
            GLfloat cosPhi = std::cos(phi);

            GLfloat x = cosPhi * sinTheta;
            GLfloat y = cosTheta;
            GLfloat z = sinPhi * sinTheta;

            glm::vec3 expectedVertex = glm::vec3(x, y, z) * radius;
            assert(compareVec3(vertices[lat * (slices + 1) + lon],
                               expectedVertex));
        }
    }
}

// Test function for `genIndices`
void testGenIndicesSphere() {
    GLfloat radius = 1.0f;
    GLint slices = 4;

    Sphere sphere(radius, slices);
    std::vector<GLuint> indices = sphere.genIndices();

    // Checking if the number of indexes is correct
    GLuint expectedIndices = slices * slices * 6; // each face has 6 indexes
    assert(indices.size() == expectedIndices);

    // Checking if indexes are within bounds
    for (size_t i = 0; i < indices.size(); ++i) {
        assert(indices[i] <
               static_cast<std::size_t>((slices + 1) * (slices + 1)));
    }
}

// Main function to run all tests
int runAllTestsSphere() {
    testGenVerticesSphere();
    testGenIndicesSphere();

    std::cout << "All Sphere Tests Passed.\n";
    return 0;
}
