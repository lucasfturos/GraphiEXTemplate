#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
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