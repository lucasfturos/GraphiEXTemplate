#include <iostream>

// Common
int runAllTestsColor();
int runAllTestsPerlin();
int runAllTestsUtil();

// Objects3D
int runAllTestsCylinder();
int runAllTestsPlane();
int runAllTestsSphere();

int main(void) {
    std::cout << "Running Color Tests...\n";
    runAllTestsColor();

    std::cout << "Running Perlin Tests...\n";
    runAllTestsPerlin();

    std::cout << "Running Util Functions Tests...\n";
    runAllTestsUtil();

    std::cout << "Running Cylinder tests...\n";
    runAllTestsCylinder();

    std::cout << "Running Plane tests...\n";
    runAllTestsPlane();

    std::cout << "Running Sphere tests...\n";
    runAllTestsSphere();

    return 0;
}