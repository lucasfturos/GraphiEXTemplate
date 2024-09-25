#pragma once

#include "GLObjects/vertex_buffer_layout.hpp"
#include "index_buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"

#include <functional>
#include <memory>
#include <unordered_map>

template <typename VertexType = glm::vec3, typename FaceType = glm::ivec3,
          typename TexType = glm::vec2, typename NormalType = glm::vec3>
class Mesh {
  public:
    Mesh(const std::vector<VertexType> &vertices,
         const std::vector<GLuint> &faces, const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath)
        : Mesh(vertices, faces, {}, {}, vertexShaderPath, fragmentShaderPath) {}

    Mesh(const std::vector<VertexType> &vertices,
         const std::vector<GLuint> &faces,
         const std::vector<TexType> &texCoords,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath)
        : Mesh(vertices, faces, {}, texCoords, vertexShaderPath,
               fragmentShaderPath) {}

    Mesh(const std::vector<VertexType> &vertices,
         const std::vector<GLuint> &faces,
         const std::vector<NormalType> &normals,
         const std::vector<TexType> &texCoords,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath)
        : va(std::make_shared<VertexArray>()),
          vb(std::make_shared<VertexBuffer<VertexType>>(vertices)),
          ib(std::make_shared<IndexBuffer>(faces)),
          nb(normals.empty()
                 ? nullptr
                 : std::make_shared<VertexBuffer<NormalType>>(normals)),
          tb(texCoords.empty()
                 ? nullptr
                 : std::make_shared<VertexBuffer<TexType>>(texCoords)),
          shader(
              std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath)),
          hasTexture(false) {}

    using VertexBufferLayoutMap = std::unordered_map<
        std::string, std::function<void(std::shared_ptr<VertexBufferLayout>)>>;

    void setup(const VertexBufferLayoutMap &layoutMap) {
        if (vb) {
            auto vertexLayout = std::make_shared<VertexBufferLayout>();
            layoutMap.at("vertices")(vertexLayout);
            va->addBuffer(*vb, *vertexLayout);
        }

        if (nb) {
            auto normalLayout = std::make_shared<VertexBufferLayout>();
            layoutMap.at("normals")(normalLayout);
            va->addBuffer(*nb, *normalLayout);
        }

        if (tb) {
            auto texCoordLayout = std::make_shared<VertexBufferLayout>();
            layoutMap.at("texCoords")(texCoordLayout);
            va->addBuffer(*tb, *texCoordLayout);
        }
    }

    using UniformsMap =
        std::unordered_map<std::string,
                           std::function<void(std::shared_ptr<Shader>)>>;

    void setUniforms(const UniformsMap &uniforms) {
        shader->bind();
        for (const auto &[name, setUniform] : uniforms) {
            setUniform(shader);
        }
        shader->unbind();
    }

    void draw() {
        shader->bind();
        va->bind();
        vb->bind();
        ib->bind();

        if (hasTexture) {
            for (auto i = 0U; i < textures.size(); ++i) {
                if (textures[i]) {
                    textures[i]->bind(i);
                }
            }
        }

        glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr);

        va->unbind();
        vb->unbind();
        ib->unbind();
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
    std::shared_ptr<VertexArray> va;
    std::shared_ptr<VertexBuffer<VertexType>> vb;
    std::shared_ptr<IndexBuffer> ib;
    std::shared_ptr<VertexBuffer<NormalType>> nb;
    std::shared_ptr<VertexBuffer<TexType>> tb;
    std::shared_ptr<Shader> shader;
    std::vector<std::shared_ptr<Texture>> textures;

    bool hasTexture;
};
