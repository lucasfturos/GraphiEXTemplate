#pragma once

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
};

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