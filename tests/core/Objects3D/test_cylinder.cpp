#include "Objects3D/cylinder.hpp"
#include <iostream>

// Declaration of utility functions
bool compareInt(int, int);
bool compareVec3(const glm::vec3 &, const glm::vec3 &, float = 0.01f);

// Test function for `genVertices`
void testGenVerticesCylinder() {
    GLfloat height = 2.0f;
    GLfloat radiusTop = 1.0f;
    GLfloat radiusBottom = 0.5f;
    GLint slices = 4;

    Cylinder cylinder(height, radiusTop, radiusBottom, slices);
    std::vector<glm::vec3> vertices = cylinder.genVertices();

    // Checking if the number of vertices is correct (2 * slices + 2, since
    // includes the center of the base and top)
    assert(vertices.size() == static_cast<std::size_t>((slices * 2) + 2));

    // Checking if the vertex coordinates are correct
    for (int i = 0; i < slices; ++i) {
        GLfloat angle = i * (2 * M_PI) / slices;

        // Upper vertices
        glm::vec3 expectedTop =
            glm::vec3(radiusTop * std::cos(angle), radiusTop * std::sin(angle),
                      height / 2.0f);
        assert(compareVec3(vertices[i * 2], expectedTop));

        // Lower vertices
        glm::vec3 expectedBottom =
            glm::vec3(radiusBottom * std::cos(angle),
                      radiusBottom * std::sin(angle), -height / 2.0f);
        assert(compareVec3(vertices[i * 2 + 1], expectedBottom));
    }
}

// Test function for `genIndices`
void testGenIndicesCylinder() {
    GLfloat height = 2.0f;
    GLfloat radiusTop = 1.0f;
    GLfloat radiusBottom = 0.5f;
    GLint slices = 4;

    Cylinder cylinder(height, radiusTop, radiusBottom, slices);
    std::vector<GLuint> indices = cylinder.genIndices();

    // Checking if the number of indexes is correct
    GLuint expectedIndices = slices * 6 + slices * 3 + slices * 3;
    assert(indices.size() == expectedIndices);

    // Checking if indexes are within bounds
    for (size_t i = 0; i < indices.size(); ++i) {
        assert(indices[i] < static_cast<std::size_t>(slices * 2 + 2));
    }
}

// Main function to run all tests
int runAllTestsCylinder() {
    testGenVerticesCylinder();
    testGenIndicesCylinder();

    std::cout << "All Cylinder Tests Passed.\n";
    return 0;
}
