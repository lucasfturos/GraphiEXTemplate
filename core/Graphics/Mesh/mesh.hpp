#pragma once

#include "Graphics/Shader/shader.hpp"
#include "Graphics/Texture/texture.hpp"
#include "Graphics/index_buffer.hpp"
#include "Graphics/vertex_array.hpp"

#include <functional>
#include <memory>
#include <unordered_map>

template <typename T, int Size> struct LayoutAttribute {
    static void setup(std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<T>(Size);
    }
};

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
         const ShaderPaths &vertexShaderPaths,
         const ShaderPaths &fragmentShaderPaths)
        : Mesh(vertices, faces, {}, {}, vertexShaderPaths,
               fragmentShaderPaths) {}

    Mesh(const std::vector<VerticesType> &vertices,
         const std::vector<FaceType> &faces,
         const std::vector<TexType> &texCoords,
         const ShaderPaths &vertexShaderPaths,
         const ShaderPaths &fragmentShaderPaths)
        : Mesh(vertices, faces, {}, texCoords, vertexShaderPaths,
               fragmentShaderPaths) {}

    Mesh(const std::vector<VerticesType> &vertices,
         const std::vector<FaceType> &faces,
         const std::vector<NormalType> &normals,
         const std::vector<TexType> &texCoords,
         const ShaderPaths &vertexShaderPaths,
         const ShaderPaths &fragmentShaderPaths)
        : Mesh(vertices, faces, normals, texCoords, {}, {}, vertexShaderPaths,
               fragmentShaderPaths) {}

    Mesh(const std::vector<VerticesType> &vertices,
         const std::vector<FaceType> &faces,
         const std::vector<TexType> &texCoords,
         const std::vector<BoneIdType> &boneIds,
         const std::vector<WeightType> &weights,
         const ShaderPaths &vertexShaderPaths,
         const ShaderPaths &fragmentShaderPaths)
        : Mesh(vertices, faces, {}, texCoords, boneIds, weights,
               vertexShaderPaths, fragmentShaderPaths) {}

    Mesh(const std::vector<VerticesType> &vertices,
         const std::vector<GLuint> &faces,
         const std::vector<NormalType> &normals,
         const std::vector<TexType> &texCoords,
         const std::vector<BoneIdType> &boneIds,
         const std::vector<WeightType> &weights,
         const ShaderPaths &vertexShaderPaths,
         const ShaderPaths &fragmentShaderPaths)
        : m_VertexArray(std::make_shared<VertexArray>()),
          m_VerticesBuffer(
              std::make_shared<VertexBuffer<VerticesType>>(vertices)),
          m_FacesBuffer(std::make_shared<IndexBuffer>(faces)),
          m_NormalBuffer(
              normals.empty()
                  ? nullptr
                  : std::make_shared<VertexBuffer<NormalType>>(normals)),
          m_TextureBuffer(
              texCoords.empty()
                  ? nullptr
                  : std::make_shared<VertexBuffer<TexType>>(texCoords)),
          m_BoneIDBuffer(
              boneIds.empty()
                  ? nullptr
                  : std::make_shared<VertexBuffer<BoneIdType>>(boneIds)),
          m_WeightBuffer(
              weights.empty()
                  ? nullptr
                  : std::make_shared<VertexBuffer<WeightType>>(weights)),
          m_Shader(
              std::make_shared<Shader>(vertexShaderPaths, fragmentShaderPaths)),
          m_HasTexture(false) {}

    using VertexBufferLayoutMap = std::unordered_map<
        std::string, std::function<void(std::shared_ptr<VertexBufferLayout>)>>;
    void setup(const VertexBufferLayoutMap &);

    using UniformsMap =
        std::unordered_map<std::string,
                           std::function<void(std::shared_ptr<Shader>)>>;
    void setUniforms(const UniformsMap &);

    void draw();
    void updateVertices(const std::vector<VerticesType> &);
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
