#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
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

template <typename Vec3Type, typename FaceType, typename NormalType,
          typename TexCoordType>
struct MeshData {
    std::vector<Vec3Type> vertices;
    std::vector<FaceType> faces;
    std::vector<NormalType> normals;
    std::vector<TexCoordType> textures;
};

using MeshDataModel = MeshData<glm::vec3, unsigned int, glm::vec3, glm::vec2>;
using MeshDataObj = MeshData<glm::vec3, glm::ivec3, glm::vec3, glm::vec2>;

inline std::string to_string(const glm::vec3 &vec) {
    std::stringstream ss;
    ss << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return ss.str();
}

inline std::string to_string(const glm::mat4 &mat) {
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