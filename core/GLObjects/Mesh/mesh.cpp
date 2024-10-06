#include "mesh.hpp"

template <typename Types>
Mesh<Types>::Mesh(const std::vector<VertexType> &vertices,
                  const std::vector<FaceType> &faces,
                  const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath)
    : Mesh(vertices, faces, {}, {}, vertexShaderPath, fragmentShaderPath) {}

template <typename Types>
Mesh<Types>::Mesh(const std::vector<VertexType> &vertices,
                  const std::vector<FaceType> &faces,
                  const std::vector<TexType> &texCoords,
                  const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath)
    : Mesh(vertices, faces, {}, texCoords, vertexShaderPath,
           fragmentShaderPath) {}

template <typename Types>
Mesh<Types>::Mesh(const std::vector<VertexType> &vertices,
                  const std::vector<FaceType> &faces,
                  const std::vector<NormalType> &normals,
                  const std::vector<TexType> &texCoords,
                  const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath)
    : Mesh(vertices, faces, normals, texCoords, {}, {}, vertexShaderPath,
           fragmentShaderPath) {}

template <typename Types>
Mesh<Types>::Mesh(const std::vector<VertexType> &vertices,
                  const std::vector<FaceType> &faces,
                  const std::vector<TexType> &texCoords,
                  const std::vector<BoneIdType> &boneIds,
                  const std::vector<WeightType> &weights,
                  const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath)
    : Mesh(vertices, faces, {}, texCoords, boneIds, weights, vertexShaderPath,
           fragmentShaderPath) {}

template <typename Types>
Mesh<Types>::Mesh(const std::vector<VertexType> &vertices,
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
      normalBuffer(normals.empty()
                       ? nullptr
                       : std::make_shared<VertexBuffer<NormalType>>(normals)),
      textureBuffer(texCoords.empty()
                        ? nullptr
                        : std::make_shared<VertexBuffer<TexType>>(texCoords)),
      boneIDBuffer(boneIds.empty()
                       ? nullptr
                       : std::make_shared<VertexBuffer<BoneIdType>>(boneIds)),
      weightBuffer(weights.empty()
                       ? nullptr
                       : std::make_shared<VertexBuffer<WeightType>>(weights)),
      shader(std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath)),
      hasTexture(false) {}

template <typename Types>
void Mesh<Types>::setup(const VertexBufferLayoutMap &layoutMap) {
    if (vertexBuffer)
        setupBuffers<VertexType>(layoutMap, "vertices", vertexBuffer, 0);
    if (normalBuffer)
        setupBuffers<NormalType>(layoutMap, "normals", normalBuffer, 1);
    if (textureBuffer)
        setupBuffers<TexType>(layoutMap, "texCoords", textureBuffer, 2);
    if (boneIDBuffer)
        setupBuffers<BoneIdType>(layoutMap, "boneIDs", boneIDBuffer, 3);
    if (weightBuffer)
        setupBuffers<WeightType>(layoutMap, "weights", weightBuffer, 4);
}

template <typename Types>
void Mesh<Types>::setUniforms(const UniformsMap &uniforms) {
    if (shader) {
        shader->bind();
        for (const auto &[name, setUniform] : uniforms) {
            setUniform(shader);
        }
        shader->unbind();
    }
}

template <typename Types> void Mesh<Types>::draw() {
    bindShader();
    bindBuffers();
    bindTextures();

    glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT,
                   nullptr);

    unbindTextures();
    unbindBuffers();
    unbindShader();
}

template <typename Types>
void Mesh<Types>::updateTexture(const std::vector<float> &data, int width,
                                int height, int depth,
                                std::uint32_t textureIndex) {
    if (textureIndex < textures.size() && textures[textureIndex]) {
        textures[textureIndex]->updateData(data, width, height, depth, GL_RGB,
                                           GL_FLOAT);
    }
}

template <typename Types>
void Mesh<Types>::setTexture(std::shared_ptr<Texture> newTexture) {
    textures.push_back(newTexture);
    hasTexture = true;
}

template <typename Types>
template <typename BufferType>
void Mesh<Types>::setupBuffers(
    const VertexBufferLayoutMap &layoutMap, const std::string &bufferName,
    const std::shared_ptr<VertexBuffer<BufferType>> &buffer,
    GLuint attributeID) {
    auto layout = std::make_shared<VertexBufferLayout>();
    if (layoutMap.find(bufferName) != layoutMap.end())
        layoutMap.at(bufferName)(layout);
    vertexArray->addBuffer(*buffer, *layout, attributeID);
}

template <typename Types> void Mesh<Types>::bindShader() {
    if (shader)
        shader->bind();
}

template <typename Types> void Mesh<Types>::unbindShader() {
    if (shader)
        shader->unbind();
}

template <typename Types> void Mesh<Types>::bindBuffers() {
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

template <typename Types> void Mesh<Types>::unbindBuffers() {
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

template <typename Types> void Mesh<Types>::bindTextures() {
    if (hasTexture) {
        for (auto i = 0U; i < textures.size(); ++i) {
            if (textures[i]) {
                textures[i]->bind(i);
            }
        }
    }
}

template <typename Types> void Mesh<Types>::unbindTextures() {
    if (hasTexture) {
        for (auto i = 0U; i < textures.size(); ++i) {
            if (textures[i]) {
                textures[i]->unbind();
            }
        }
    }
}