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

          normalBuffer(std::make_shared<VertexBuffer<NormalType>>(normals)),
          textureBuffer(std::make_shared<VertexBuffer<TexType>>(texCoords)),

          boneIDBuffer(std::make_shared<VertexBuffer<BoneIdType>>(boneIds)),
          weightBuffer(std::make_shared<VertexBuffer<WeightType>>(weights)),

          shader(
              std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath)),
          hasTexture(false) {}

    using VertexBufferLayoutMap = std::unordered_map<
        std::string, std::function<void(std::shared_ptr<VertexBufferLayout>)>>;

    void setup(const VertexBufferLayoutMap &layoutMap) {
        if (vertexBuffer) {
            auto vertexLayout = std::make_shared<VertexBufferLayout>();
            if (layoutMap.find("vertices") != layoutMap.end())
                layoutMap.at("vertices")(vertexLayout);
            vertexArray->addBuffer(*vertexBuffer, *vertexLayout);
        }

        if (normalBuffer) {
            auto normalLayout = std::make_shared<VertexBufferLayout>();
            if (layoutMap.find("normals") != layoutMap.end())
                layoutMap.at("normals")(normalLayout);
            vertexArray->addBuffer(*normalBuffer, *normalLayout);
        }

        if (textureBuffer) {
            auto texCoordLayout = std::make_shared<VertexBufferLayout>();
            if (layoutMap.find("texCoords") != layoutMap.end())
                layoutMap.at("texCoords")(texCoordLayout);
            vertexArray->addBuffer(*textureBuffer, *texCoordLayout);
        }

        if (boneIDBuffer) {
            auto boneIdLayout = std::make_shared<VertexBufferLayout>();
            if (layoutMap.find("boneIDs") != layoutMap.end())
                layoutMap.at("boneIDs")(boneIdLayout);
            vertexArray->addBuffer(*boneIDBuffer, *boneIdLayout);
        }

        if (weightBuffer) {
            auto weightLayout = std::make_shared<VertexBufferLayout>();
            if (layoutMap.find("weights") != layoutMap.end())
                layoutMap.at("weights")(weightLayout);
            vertexArray->addBuffer(*weightBuffer, *weightLayout);
        }
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
        if (shader)
            shader->bind();

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

        if (hasTexture) {
            for (auto i = 0U; i < textures.size(); ++i) {
                if (textures[i]) {
                    textures[i]->bind(i);
                }
            }
        }

        glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT,
                       nullptr);

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

        if (shader)
            shader->unbind();

        if (hasTexture) {
            for (auto i = 0U; i < textures.size(); ++i) {
                if (textures[i]) {
                    textures[i]->unbind();
                }
            }
        }
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

    std::shared_ptr<Texture> getTexture(std::uint32_t index) {
        if (index < textures.size()) {
            return textures[index];
        }
        return nullptr;
    }

  private:
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
