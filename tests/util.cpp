#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Comparison function for integers
bool compareInt(int a, int b) { return a == b; }

// Comparison function with tolerance for floats
bool compareFloats(float a, float b, float tolerance = 0.01f) {
    return std::fabs(a - b) < tolerance;
}

// Helper function to compare two `glm::vec3`
bool compareColors(const glm::vec3 &color1, const glm::vec3 &color2,
                   float tolerance = 0.01f) {
    return compareFloats(color1.r, color2.r, tolerance) &&
           compareFloats(color1.g, color2.g, tolerance) &&
           compareFloats(color1.b, color2.b, tolerance);
}

// Comparison function for 4x4 matrices
bool compareMatrices(const glm::mat4 &mat1, const glm::mat4 &mat2,
                     float tolerance = 0.01f) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (!compareFloats(mat1[i][j], mat2[i][j], tolerance))
                return false;
        }
    }
    return true;
}

// Comparison function for vectors x, y, z
bool compareVec3(const glm::vec3 &a, const glm::vec3 &b,
                 float tolerance = 0.01f) {
    return compareFloats(a.x, b.x, tolerance) &&
           compareFloats(a.y, b.y, tolerance) &&
           compareFloats(a.z, b.z, tolerance);
}

// Comparison function for quaternion
bool compareQuat(const glm::quat &a, const glm::quat &b,
                 float tolerance = 0.01f) {
    return compareFloats(a.x, b.x, tolerance) &&
           compareFloats(a.y, b.y, tolerance) &&
           compareFloats(a.z, b.z, tolerance) &&
           compareFloats(a.w, b.w, tolerance);
}
