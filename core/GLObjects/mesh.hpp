#pragma once

#include "GLObjects/vertex_buffer_layout.hpp"
#include "index_buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"

#include <functional>
#include <memory>
#include <unordered_map>

template <typename VertexType = glm::vec3, typename FaceType = GLuint,
          typename TexType = glm::vec2, typename NormalType = glm::vec3,
          typename BoneIdType = GLint, typename WeightType = GLuint>
class Mesh {
  public:
    Mesh(const std::vector<VertexType> &vertices,
         const std::vector<FaceType> &faces,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath)
        : Mesh(vertices, faces, {}, {}, vertexShaderPath, fragmentShaderPath) {}

    Mesh(const std::vector<VertexType> &vertices,
         const std::vector<FaceType> &faces,
         const std::vector<TexType> &texCoords,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath)
        : Mesh(vertices, faces, {}, texCoords, vertexShaderPath,
               fragmentShaderPath) {}

    Mesh(const std::vector<VertexType> &vertices,
         const std::vector<FaceType> &faces,
         const std::vector<NormalType> &normals,
         const std::vector<TexType> &texCoords,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath)
        : Mesh(vertices, faces, normals, texCoords, {}, {}, vertexShaderPath,
               fragmentShaderPath) {}

    Mesh(const std::vector<VertexType> &vertices,
         const std::vector<FaceType> &faces,
         const std::vector<TexType> &texCoords,
         const std::vector<BoneIdType> &boneIds,
         const std::vector<WeightType> &weights,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath)
        : Mesh(vertices, faces, {}, texCoords, boneIds, weights,
               vertexShaderPath, fragmentShaderPath) {}

    Mesh(const std::vector<VertexType> &vertices,
         const std::vector<GLuint> &faces,
         const std::vector<NormalType> &normals,
         const std::vector<TexType> &texCoords,
         const std::vector<BoneIdType> &boneIds,
         const std::vector<WeightType> &weights,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath)
        : vertexArray(std::make_shared<VertexArray>()),
          vertexBuffer(std::make_shared<VertexBuffer<VertexType>>(vertices)),
          indexBuffer(std::make_shared<IndexBuffer>(faces)),
          normalBuffer(
              normals.empty()
                  ? nullptr
                  : std::make_shared<VertexBuffer<NormalType>>(normals)),
          textureBuffer(
              texCoords.empty()
                  ? nullptr
                  : std::make_shared<VertexBuffer<TexType>>(texCoords)),
          boneIDBuffer(
              boneIds.empty()
                  ? nullptr
                  : std::make_shared<VertexBuffer<BoneIdType>>(boneIds)),
          weightBuffer(
              weights.empty()
                  ? nullptr
                  : std::make_shared<VertexBuffer<WeightType>>(weights)),
          shader(
              std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath)),
          hasTexture(false) {}

    using VertexBufferLayoutMap = std::unordered_map<
        std::string, std::function<void(std::shared_ptr<VertexBufferLayout>)>>;

    void setup(const VertexBufferLayoutMap &layoutMap) {
        if (vertexBuffer)
            setupBuffers<VertexType>(layoutMap, "vertices", vertexBuffer);
        if (normalBuffer)
            setupBuffers<NormalType>(layoutMap, "normals", normalBuffer);
        if (textureBuffer)
            setupBuffers<TexType>(layoutMap, "texCoords", textureBuffer);
        if (boneIDBuffer)
            setupBuffers<BoneIdType>(layoutMap, "boneIDs", boneIDBuffer);
        if (weightBuffer)
            setupBuffers<WeightType>(layoutMap, "weights", weightBuffer);
    }

    using UniformsMap =
        std::unordered_map<std::string,
                           std::function<void(std::shared_ptr<Shader>)>>;

    void setUniforms(const UniformsMap &uniforms) {
        if (shader) {
            shader->bind();
            for (const auto &[name, setUniform] : uniforms) {
                setUniform(shader);
            }
            shader->unbind();
        }
    }

    void draw() {
        bindShader();
        bindBuffers();
        bindTextures();

        glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT,
                       nullptr);

        unbindTextures();
        unbindBuffers();
        unbindShader();
    }

    void updateTexture(const std::vector<float> &data, int width, int height,
                       int depth, std::uint32_t textureIndex = 0) {
        if (textureIndex < textures.size() && textures[textureIndex]) {
            textures[textureIndex]->updateData(data, width, height, depth,
                                               GL_RGB, GL_FLOAT);
        }
    }

    void setTexture(std::shared_ptr<Texture> newTexture) {
        textures.push_back(newTexture);
        hasTexture = true;
    }

  private:
    template <typename BufferType>
    void setupBuffers(const VertexBufferLayoutMap &layoutMap,
                      const std::string &bufferName,
                      const std::shared_ptr<VertexBuffer<BufferType>> &buffer) {
        auto layout = std::make_shared<VertexBufferLayout>();
        if (layoutMap.find(bufferName) != layoutMap.end())
            layoutMap.at(bufferName)(layout);
        vertexArray->addBuffer(*buffer, *layout);
    }

    void bindShader() {
        if (shader)
            shader->bind();
    }

    void unbindShader() {
        if (shader)
            shader->unbind();
    }

    void bindBuffers() {
        if (vertexArray)
            vertexArray->bind();
        if (vertexBuffer)
            vertexBuffer->bind();
        if (indexBuffer)
            indexBuffer->bind();
        if (boneIDBuffer)
            boneIDBuffer->bind();
        if (weightBuffer)
            weightBuffer->bind();
    }

    void unbindBuffers() {
        if (vertexArray)
            vertexArray->unbind();
        if (vertexBuffer)
            vertexBuffer->unbind();
        if (indexBuffer)
            indexBuffer->unbind();
        if (boneIDBuffer)
            boneIDBuffer->unbind();
        if (weightBuffer)
            weightBuffer->unbind();
    }

    void bindTextures() {
        if (hasTexture) {
            for (auto i = 0U; i < textures.size(); ++i) {
                if (textures[i]) {
                    textures[i]->bind(i);
                }
            }
        }
    }

    void unbindTextures() {
        if (hasTexture) {
            for (auto i = 0U; i < textures.size(); ++i) {
                if (textures[i]) {
                    textures[i]->unbind();
                }
            }
        }
    }

    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;
    std::shared_ptr<VertexBuffer<NormalType>> normalBuffer;
    std::shared_ptr<VertexBuffer<TexType>> textureBuffer;

    std::shared_ptr<VertexBuffer<BoneIdType>> boneIDBuffer;
    std::shared_ptr<VertexBuffer<WeightType>> weightBuffer;

    std::shared_ptr<Shader> shader;
    std::vector<std::shared_ptr<Texture>> textures;

    bool hasTexture;
};
