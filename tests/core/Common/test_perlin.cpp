#include "Common/perlin.hpp"
#include <cassert>
#include <iostream>

// Test function for the noise method
void testPerlinNoise() {
    Perlin perlin;
    glm::vec3 point(1.5f, 2.3f, 3.7f);
    double noiseValue = perlin.noise(point);

    assert(noiseValue >= -1.0 && noiseValue <= 1.0);
}

// Test function for the turb method
void testPerlinTurb() {
    Perlin perlin;
    glm::vec3 point(2.1f, 1.7f, 0.5f);
    double turbValue = perlin.turb(point);

    assert(turbValue >= 0.0);
}

// Main function to run all tests
int runAllTestsPerlin() {
    testPerlinNoise();
    testPerlinTurb();

    std::cout << "All Perlin Tests Passed.\n";
    return 0;
}
