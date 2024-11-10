#include "Common/color.hpp"
#include <iostream>

// Declaration of Utility functions
bool compareColors(const glm::vec3 &, const glm::vec3 &, float = 0.01f);

// Testing the HSV2RGB function with specific values
bool testHSV2RGB() {
    // Test 1: Pure red
    glm::vec3 result = HSV2RGB(0.0f, 1.0f, 1.0f);
    glm::vec3 expected = {1.0f, 0.0f, 0.0f};
    if (!compareColors(result, expected)) {
        std::cerr << "Test failed for HSV2RGB(0.0, 1.0, 1.0): Expected "
                  << expected.r << ", " << expected.g << ", " << expected.b
                  << " but got " << result.r << ", " << result.g << ", "
                  << result.b << "\n";
        return false;
    }

    // Test 2: Pure green
    result = HSV2RGB(1.0f / 3.0f, 1.0f, 1.0f);
    expected = {0.0f, 1.0f, 0.0f};
    if (!compareColors(result, expected)) {
        std::cerr << "Test failed for HSV2RGB(1/3, 1.0, 1.0): Expected "
                  << expected.r << ", " << expected.g << ", " << expected.b
                  << " but got " << result.r << ", " << result.g << ", "
                  << result.b << "\n";
        return false;
    }

    // Test 3: Pure Blue
    result = HSV2RGB(2.0f / 3.0f, 1.0f, 1.0f);
    expected = {0.0f, 0.0f, 1.0f};
    if (!compareColors(result, expected)) {
        std::cerr << "Test failed for HSV2RGB(2/3, 1.0, 1.0): Expected "
                  << expected.r << ", " << expected.g << ", " << expected.b
                  << " but got " << result.r << ", " << result.g << ", "
                  << result.b << "\n";
        return false;
    }

    // Test 4: White (max brightness, zero saturation)
    result = HSV2RGB(0.0f, 0.0f, 1.0f);
    expected = {1.0f, 1.0f, 1.0f};
    if (!compareColors(result, expected)) {
        std::cerr << "Test failed for HSV2RGB(0.0, 0.0, 1.0): Expected "
                  << expected.r << ", " << expected.g << ", " << expected.b
                  << " but got " << result.r << ", " << result.g << ", "
                  << result.b << "\n";
        return false;
    }

    return true;
}

// Test for getColorByValue function with specific values
bool testGetColorByValue() {
    // Test 1: Minimum value (0) should result in red
    glm::vec3 result = getColorByValue(0.0f);
    glm::vec3 expected = HSV2RGB(0.0f, 0.8f, 0.8f);
    if (!compareColors(result, expected)) {
        std::cerr << "Test failed for getColorByValue(0.0): Expected "
                  << expected.r << ", " << expected.g << ", " << expected.b
                  << " but got " << result.r << ", " << result.g << ", "
                  << result.b << "\n";
        return false;
    }

    // Test 2: Maximum value (255) should result in a color close to red
    result = getColorByValue(255.0f);
    expected = HSV2RGB(1.0f, 0.8f, 0.8f);
    if (!compareColors(result, expected)) {
        std::cerr << "Test failed for getColorByValue(255.0): Expected "
                  << expected.r << ", " << expected.g << ", " << expected.b
                  << " but got " << result.r << ", " << result.g << ", "
                  << result.b << "\n";
        return false;
    }

    // Test 3: Intermediate value (127) should result in a cyan-green color
    result = getColorByValue(127.0f);
    expected = HSV2RGB(127.0f / 255.0f, 0.8f, 0.8f);
    if (!compareColors(result, expected)) {
        std::cerr << "Test failed for getColorByValue(127.0): Expected "
                  << expected.r << ", " << expected.g << ", " << expected.b
                  << " but got " << result.r << ", " << result.g << ", "
                  << result.b << "\n";
        return false;
    }

    return true;
}

// Main function to run all tests
int runAllTestsColor() {
    bool allPassed = true;

    if (!testHSV2RGB()) {
        std::cerr << "testHSV2RGB Failed!\n";
        allPassed = false;
    }

    if (!testGetColorByValue()) {
        std::cerr << "testGetColorByValue Failed!\n";
        allPassed = false;
    }

    if (allPassed) {
        std::cout << "All Color Tests Passed.\n";
        return 0;
    } else {
        std::cerr << "Some Color Tests Failed.\n";
        return 1;
    }
}
