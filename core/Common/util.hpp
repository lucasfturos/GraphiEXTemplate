#pragma once

#include <GL/glew.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>

#include <sstream>
#include <string>
#include <vector>

enum class ObjectType {
    None,
    Sphere,
    Cylinder,
    Plane,
    Cube,
    FunctionPlot,
};

namespace DebugGLM {

inline std::string vec3ToString(const glm::vec3 &vec) {
    std::stringstream ss;
    ss << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return ss.str();
}

inline std::string vec4ToString(const glm::vec4 &vec) {
    std::stringstream ss;
    ss << "vec4(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w
       << ")";
    return ss.str();
}

inline std::string mat4ToString(const glm::mat4 &mat) {
    std::stringstream ss;
    ss << "mat4(";
    for (int i = 0; i < 4; ++i) {
        ss << "(" << mat[i][0] << ", " << mat[i][1] << ", " << mat[i][2] << ", "
           << mat[i][3] << ")";
        if (i < 3)
            ss << ", ";
    }
    ss << ")";
    return ss.str();
}

} // namespace DebugGLM

namespace AssimpGLMConversion {

inline glm::mat4 aiMatrix4x4ToGLM(const aiMatrix4x4 &from) {
    return glm::mat4(from.a1, from.b1, from.c1, from.d1, from.a2, from.b2,
                     from.c2, from.d2, from.a3, from.b3, from.c3, from.d3,
                     from.a4, from.b4, from.c4, from.d4);
}

inline glm::vec3 aiVector3DToGLM(const aiVector3D &vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}

inline glm::quat aiQuaternionToGLM(const aiQuaternion &pOrientation) {
    return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y,
                     pOrientation.z);
}

} // namespace AssimpGLMConversion

namespace VolumeGeneration {

inline std::vector<GLfloat> generateTransferFunction(int width) {
    std::vector<GLfloat> transferFunction(width * 4);
    for (int i = 0; i < width; ++i) {
        float density = static_cast<float>(i) / static_cast<float>(width - 1);
        transferFunction[i * 4 + 0] = density;
        transferFunction[i * 4 + 1] = density;
        transferFunction[i * 4 + 2] = density;
        transferFunction[i * 4 + 3] = density;
    }
    return transferFunction;
}

inline std::vector<GLfloat>
generateDensityData(const std::vector<glm::vec3> &vertices,
                    const std::vector<GLuint> &faces, int width, int height,
                    int depth) {
    std::vector<GLfloat> densityData(width * height * depth, 0.0f);
    for (size_t i = 0; i < faces.size(); i += 3) {
        glm::vec3 v0 = vertices[faces[i + 0]];
        glm::vec3 v1 = vertices[faces[i + 1]];
        glm::vec3 v2 = vertices[faces[i + 2]];

        for (const auto &vertex : {v0, v1, v2}) {
            float x = (vertex.x + 1.0f) / 2.0f * (width - 1);
            float y = (vertex.y + 1.0f) / 2.0f * (height - 1);
            float z = (vertex.z + 1.0f) / 2.0f * (depth - 1);

            int xi = static_cast<int>(x);
            int yi = static_cast<int>(y);
            int zi = static_cast<int>(z);

            float dx = x - xi;
            float dy = y - yi;
            float dz = z - zi;

            if (xi >= 0 && xi < width - 1 && yi >= 0 && yi < height - 1 &&
                zi >= 0 && zi < depth - 1) {
                densityData[xi + width * (yi + height * zi)] +=
                    (1 - dx) * (1 - dy) * (1 - dz);
                densityData[(xi + 1) + width * (yi + height * zi)] +=
                    dx * (1 - dy) * (1 - dz);
                densityData[xi + width * ((yi + 1) + height * zi)] +=
                    (1 - dx) * dy * (1 - dz);
                densityData[xi + width * (yi + height * (zi + 1))] +=
                    (1 - dx) * (1 - dy) * dz;
                densityData[(xi + 1) + width * ((yi + 1) + height * zi)] +=
                    dx * dy * (1 - dz);
                densityData[xi + width * ((yi + 1) + height * (zi + 1))] +=
                    (1 - dx) * dy * dz;
                densityData[(xi + 1) + width * (yi + height * (zi + 1))] +=
                    dx * (1 - dy) * dz;
                densityData[(xi + 1) +
                            width * ((yi + 1) + height * (zi + 1))] +=
                    dx * dy * dz;
            }
        }
    }
    return densityData;
}

} // namespace VolumeGeneration