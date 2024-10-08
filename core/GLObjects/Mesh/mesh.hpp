#pragma once

#include "GLObjects/Texture/texture.hpp"
#include "GLObjects/index_buffer.hpp"
#include "GLObjects/shader.hpp"
#include "GLObjects/vertex_array.hpp"

#include <functional>
#include <memory>
#include <unordered_map>

template <typename VerticesType = glm::vec3, typename FaceType = GLuint,
          typename TexType = glm::vec2, typename NormalType = glm::vec3,
          typename BoneIdType = glm::ivec4, typename WeightType = glm::vec4>
struct MeshTypes {
    using Vertices = VerticesType;
    using Face = FaceType;
    using Tex = TexType;
    using Normal = NormalType;
    using BoneId = BoneIdType;
    using Weight = WeightType;
};

using Types = MeshTypes<>;

template <typename Types> class Mesh {
  public:
    using VerticesType = typename Types::Vertices;
    using FaceType = typename Types::Face;
    using TexType = typename Types::Tex;
    using NormalType = typename Types::Normal;
    using BoneIdType = typename Types::BoneId;
    using WeightType = typename Types::Weight;

    Mesh(const std::vector<VerticesType> &vertices,
         const std::vector<FaceType> &faces,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath);

    Mesh(const std::vector<VerticesType> &vertices,
         const std::vector<FaceType> &faces,
         const std::vector<TexType> &texCoords,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath);

    Mesh(const std::vector<VerticesType> &vertices,
         const std::vector<FaceType> &faces,
         const std::vector<NormalType> &normals,
         const std::vector<TexType> &texCoords,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath);

    Mesh(const std::vector<VerticesType> &vertices,
         const std::vector<FaceType> &faces,
         const std::vector<TexType> &texCoords,
         const std::vector<BoneIdType> &boneIds,
         const std::vector<WeightType> &weights,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath);

    Mesh(const std::vector<VerticesType> &vertices,
         const std::vector<GLuint> &faces,
         const std::vector<NormalType> &normals,
         const std::vector<TexType> &texCoords,
         const std::vector<BoneIdType> &boneIds,
         const std::vector<WeightType> &weights,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath);

    using VertexBufferLayoutMap = std::unordered_map<
        std::string, std::function<void(std::shared_ptr<VertexBufferLayout>)>>;
    void setup(const VertexBufferLayoutMap &);

    using UniformsMap =
        std::unordered_map<std::string,
                           std::function<void(std::shared_ptr<Shader>)>>;
    void setUniforms(const UniformsMap &);

    void draw();
    void updateTexture(const std::vector<float> &, int, int, int,
                       std::uint32_t = 0);
    void setTexture(std::shared_ptr<Texture>);

  private:
    template <typename BufferType>
    void setupBuffers(const VertexBufferLayoutMap &, const std::string &,
                      const std::shared_ptr<VertexBuffer<BufferType>> &,
                      GLuint);

    void bindShader();
    void unbindShader();

    void bindBuffers();
    void unbindBuffers();

    void bindTextures();
    void unbindTextures();

    std::shared_ptr<VertexArray> m_VertexArray;
    std::shared_ptr<VertexBuffer<VerticesType>> m_VerticesBuffer;
    std::shared_ptr<IndexBuffer> m_FacesBuffer;
    std::shared_ptr<VertexBuffer<NormalType>> m_NormalBuffer;
    std::shared_ptr<VertexBuffer<TexType>> m_TextureBuffer;

    std::shared_ptr<VertexBuffer<BoneIdType>> m_BoneIDBuffer;
    std::shared_ptr<VertexBuffer<WeightType>> m_WeightBuffer;

    std::shared_ptr<Shader> m_Shader;
    std::vector<std::shared_ptr<Texture>> m_Textures;

    bool m_HasTexture;
};

template class Mesh<Types>;
