#pragma once

#include <glm/gtc/quaternion.hpp>

#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>

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
