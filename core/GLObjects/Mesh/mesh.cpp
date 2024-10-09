#include "mesh.hpp"

template <typename Types>
Mesh<Types>::Mesh(const std::vector<VerticesType> &vertices,
                  const std::vector<FaceType> &faces,
                  const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath)
    : Mesh(vertices, faces, {}, {}, vertexShaderPath, fragmentShaderPath) {}

template <typename Types>
Mesh<Types>::Mesh(const std::vector<VerticesType> &vertices,
                  const std::vector<FaceType> &faces,
                  const std::vector<TexType> &texCoords,
                  const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath)
    : Mesh(vertices, faces, {}, texCoords, vertexShaderPath,
           fragmentShaderPath) {}

template <typename Types>
Mesh<Types>::Mesh(const std::vector<VerticesType> &vertices,
                  const std::vector<FaceType> &faces,
                  const std::vector<NormalType> &normals,
                  const std::vector<TexType> &texCoords,
                  const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath)
    : Mesh(vertices, faces, normals, texCoords, {}, {}, vertexShaderPath,
           fragmentShaderPath) {}

template <typename Types>
Mesh<Types>::Mesh(const std::vector<VerticesType> &vertices,
                  const std::vector<FaceType> &faces,
                  const std::vector<TexType> &texCoords,
                  const std::vector<BoneIdType> &boneIds,
                  const std::vector<WeightType> &weights,
                  const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath)
    : Mesh(vertices, faces, {}, texCoords, boneIds, weights, vertexShaderPath,
           fragmentShaderPath) {}

template <typename Types>
Mesh<Types>::Mesh(const std::vector<VerticesType> &vertices,
                  const std::vector<GLuint> &faces,
                  const std::vector<NormalType> &normals,
                  const std::vector<TexType> &texCoords,
                  const std::vector<BoneIdType> &boneIds,
                  const std::vector<WeightType> &weights,
                  const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath)
    : m_VertexArray(std::make_shared<VertexArray>()),
      m_VerticesBuffer(std::make_shared<VertexBuffer<VerticesType>>(vertices)),
      m_FacesBuffer(std::make_shared<IndexBuffer>(faces)),
      m_NormalBuffer(normals.empty()
                         ? nullptr
                         : std::make_shared<VertexBuffer<NormalType>>(normals)),
      m_TextureBuffer(texCoords.empty()
                          ? nullptr
                          : std::make_shared<VertexBuffer<TexType>>(texCoords)),
      m_BoneIDBuffer(boneIds.empty()
                         ? nullptr
                         : std::make_shared<VertexBuffer<BoneIdType>>(boneIds)),
      m_WeightBuffer(weights.empty()
                         ? nullptr
                         : std::make_shared<VertexBuffer<WeightType>>(weights)),
      m_Shader(std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath)),
      m_HasTexture(false) {}

template <typename Types>
void Mesh<Types>::setup(const VertexBufferLayoutMap &layoutMap) {
    if (m_VerticesBuffer)
        setupBuffers<VerticesType>(layoutMap, "vertices", m_VerticesBuffer, 0);
    if (m_NormalBuffer)
        setupBuffers<NormalType>(layoutMap, "normals", m_NormalBuffer, 1);
    if (m_TextureBuffer)
        setupBuffers<TexType>(layoutMap, "texCoords", m_TextureBuffer, 2);
    if (m_BoneIDBuffer)
        setupBuffers<BoneIdType>(layoutMap, "boneIDs", m_BoneIDBuffer, 3);
    if (m_WeightBuffer)
        setupBuffers<WeightType>(layoutMap, "weights", m_WeightBuffer, 4);
}

template <typename Types>
void Mesh<Types>::setUniforms(const UniformsMap &uniforms) {
    if (m_Shader) {
        m_Shader->bind();
        for (const auto &[name, setUniform] : uniforms) {
            setUniform(m_Shader);
        }
        m_Shader->unbind();
    }
}

template <typename Types> void Mesh<Types>::draw() {
    bindShader();
    bindBuffers();
    bindTextures();

    glDrawElements(GL_TRIANGLES, m_FacesBuffer->getCount(), GL_UNSIGNED_INT,
                   nullptr);

    unbindTextures();
    unbindBuffers();
    unbindShader();
}

template <typename Types>
void Mesh<Types>::updateTexture(const std::vector<float> &data, int width,
                                int height, int depth,
                                std::uint32_t textureIndex) {
    if (textureIndex < m_Textures.size() && m_Textures[textureIndex]) {
        m_Textures[textureIndex]->updateData(data, width, height, depth, GL_RGB,
                                             GL_FLOAT);
    }
}

template <typename Types>
void Mesh<Types>::setTexture(std::shared_ptr<Texture> newTexture) {
    m_Textures.push_back(newTexture);
    m_HasTexture = true;
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
    m_VertexArray->addBuffer(*buffer, *layout, attributeID);
}

template <typename Types> void Mesh<Types>::bindShader() {
    if (m_Shader)
        m_Shader->bind();
}

template <typename Types> void Mesh<Types>::unbindShader() {
    if (m_Shader)
        m_Shader->unbind();
}

template <typename Types> void Mesh<Types>::bindBuffers() {
    if (m_VertexArray)
        m_VertexArray->bind();
    if (m_VerticesBuffer)
        m_VerticesBuffer->bind();
    if (m_FacesBuffer)
        m_FacesBuffer->bind();
    if (m_BoneIDBuffer)
        m_BoneIDBuffer->bind();
    if (m_WeightBuffer)
        m_WeightBuffer->bind();
}

template <typename Types> void Mesh<Types>::unbindBuffers() {
    if (m_VertexArray)
        m_VertexArray->unbind();
    if (m_VerticesBuffer)
        m_VerticesBuffer->unbind();
    if (m_FacesBuffer)
        m_FacesBuffer->unbind();
    if (m_BoneIDBuffer)
        m_BoneIDBuffer->unbind();
    if (m_WeightBuffer)
        m_WeightBuffer->unbind();
}

template <typename Types> void Mesh<Types>::bindTextures() {
    if (m_HasTexture) {
        for (auto i = 0U; i < m_Textures.size(); ++i) {
            if (m_Textures[i]) {
                m_Textures[i]->bind(i);
            }
        }
    }
}

template <typename Types> void Mesh<Types>::unbindTextures() {
    if (m_HasTexture) {
        for (auto i = 0U; i < m_Textures.size(); ++i) {
            if (m_Textures[i]) {
                m_Textures[i]->unbind();
            }
        }
    }
}