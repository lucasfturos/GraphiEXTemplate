#include "Common/util.hpp"
#include <cassert>
#include <iostream>

// Declaration of Utility functions
bool compareFloats(float, float, float = 0.01f);
bool compareMatrices(const glm::mat4 &, const glm::mat4 &, float = 0.01f);

// Tests for AssimpGLMConversion
// void testAiMatrix4x4ToGLM() {
//     aiMatrix4x4 aiMat(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f,
//                       10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);
//     glm::mat4 glmMat = AssimpGLMConversion::aiMatrix4x4ToGLM(aiMat);
//     glm::mat4 expected(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f,
//                        10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);
//     assert(compareMatrices(glmMat, expected));
// }

void testAiVector3DToGLM() {
    aiVector3D aiVec(1.0f, 2.0f, 3.0f);
    glm::vec3 glmVec = AssimpGLMConversion::aiVector3DToGLM(aiVec);
    assert(compareFloats(glmVec.x, 1.0f) && compareFloats(glmVec.y, 2.0f) &&
           compareFloats(glmVec.z, 3.0f));
}

void testAiQuaternionToGLM() {
    aiQuaternion aiQuat(1.0f, 0.0f, 1.0f, 0.0f);
    glm::quat glmQuat = AssimpGLMConversion::aiQuaternionToGLM(aiQuat);
    assert(compareFloats(glmQuat.w, 1.0f) && compareFloats(glmQuat.x, 0.0f) &&
           compareFloats(glmQuat.y, 1.0f) && compareFloats(glmQuat.z, 0.0f));
}

// Tests for DebugGLM
void testVec3ToString() {
    glm::vec3 vec(1.0f, 2.0f, 3.0f);
    std::string result = DebugGLM::vec3ToString(vec);
    assert(result == "vec3(1, 2, 3)");
}

void testVec4ToString() {
    glm::vec4 vec(1.0f, 2.0f, 3.0f, 4.0f);
    std::string result = DebugGLM::vec4ToString(vec);
    assert(result == "vec4(1, 2, 3, 4)");
}

void testMat4ToString() {
    glm::mat4 mat(1.0f);
    std::string result = DebugGLM::mat4ToString(mat);
    assert(result ==
           "mat4((1, 0, 0, 0), (0, 1, 0, 0), (0, 0, 1, 0), (0, 0, 0, 1))");
}

// Tests for VolumeGeneration
void testGenerateTransferFunction() {
    int width = 5;
    auto transferFunction = VolumeGeneration::generateTransferFunction(width);
    assert(transferFunction.size() == static_cast<std::size_t>(width * 4));
    for (int i = 0; i < width; ++i) {
        float expectedDensity = static_cast<float>(i) / (width - 1);
        assert(compareFloats(transferFunction[i * 4 + 0], expectedDensity) &&
               compareFloats(transferFunction[i * 4 + 1], expectedDensity) &&
               compareFloats(transferFunction[i * 4 + 2], expectedDensity) &&
               compareFloats(transferFunction[i * 4 + 3], expectedDensity));
    }
}

// Main function to run all tests
int runAllTestsUtil() {
    testAiVector3DToGLM();
    testAiQuaternionToGLM();
    testVec3ToString();
    testVec4ToString();
    testMat4ToString();
    testGenerateTransferFunction();

    std::cout << "All Util Functions Tests Passed.\n";
    return 0;
}
