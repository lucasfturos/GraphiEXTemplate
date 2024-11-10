#include "GLObjects/interpolation.hpp"
#include <cassert>
#include <iostream>

// Declaration of utility functions
bool compareFloats(float, float, float = 0.01f);
bool compareVec3(const glm::vec3 &, const glm::vec3 &, float = 0.01f);
bool compareQuat(const glm::quat &, const glm::quat &, float = 0.01f);

// Tests for position interpolation
void testInterpolatePosition() {
    KeyPosition from = {{0.0f, 0.0f, 0.0f}, 0.0f};
    KeyPosition to = {{10.0f, 10.0f, 10.0f}, 1.0f};
    float animationTime = 0.5f; // Halfway between the two keys

    glm::mat4 result = interpolatePosition(animationTime, from, to);
    glm::vec3 expectedPosition = {5.0f, 5.0f,
                                  5.0f}; // Linear interpolation: halfway
    glm::vec3 resultPosition =
        glm::vec3(result[3]); // Get the translation of the resulting matrix

    assert(compareVec3(resultPosition, expectedPosition));
}

// Tests for rotation interpolation
void testInterpolateRotation() {
    KeyRotation from = {glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
                        0.0f}; // No rotation
    KeyRotation to = {glm::quat(glm::vec3(0.0f, glm::pi<float>(), 0.0f)),
                      1.0f};    // 180 degree rotation
    float animationTime = 0.5f; // Halfway

    glm::mat4 result = interpolateRotation(animationTime, from, to);

    // The expected rotation would be 90 degrees (halfway between 0 and 180
    // degrees)
    glm::quat expectedRotation =
        glm::quat(glm::vec3(0.0f, glm::pi<float>() / 2.0f, 0.0f));
    glm::mat4 expectedMat = glm::toMat4(expectedRotation);

    assert(compareFloats(result[0][0], expectedMat[0][0]));
    assert(compareFloats(result[1][1], expectedMat[1][1]));
    assert(compareFloats(result[2][2], expectedMat[2][2]));
    assert(compareFloats(result[3][3], expectedMat[3][3]));
}

// Tests for scale interpolation
void testInterpolateScaling() {
    KeyScale from = {{1.0f, 1.0f, 1.0f}, 0.0f};
    KeyScale to = {{2.0f, 2.0f, 2.0f}, 1.0f};
    float animationTime = 0.5f; // Halfway between the two keys

    glm::mat4 result = interpolateScaling(animationTime, from, to);

    glm::vec3 expectedScale = {1.5f, 1.5f,
                               1.5f}; // Linear interpolation: halfway
    glm::vec3 resultScale =
        glm::vec3(result[0][0], result[1][1],
                  result[2][2]); // Get the scale of the resulting matrix

    assert(compareVec3(resultScale, expectedScale));
}

int runAllTestsInterpolation() {
    testInterpolatePosition();
    testInterpolateRotation();
    testInterpolateScaling();

    std::cout << "All Interpolation Tests Passed.\n";
    return 0;
}
