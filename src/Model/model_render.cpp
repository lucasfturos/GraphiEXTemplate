#include "model_render.hpp"

ModelRender::ModelRender(const std::string &filepath)
    : m_Model(std::make_shared<Model>(filepath)),
      m_Animation(std::make_shared<Animation>(filepath, m_Model)),
      m_Animator(std::make_shared<Animator>(m_Animation)),
      m_ModelMatrix(glm::mat4(1.0f)),
      m_ProjMatrix(
          glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f)) {}

void ModelRender::setup() {
    loadModel();
    setupMesh();
    loadTextures();
    setUniforms();
}

void ModelRender::loadModel() {
    m_Vertices = m_Model->getVertices();
    m_Faces = m_Model->getFaces();
    m_Normals = m_Model->getNormals();
    m_BoneIDs = m_Model->getBoneIds();
    m_Weights = m_Model->getWeights();
}

void ModelRender::setupMesh() {
    auto texCoords = m_Model->getTexCoords();
    m_Mesh = std::make_shared<Mesh<Types>>(
        m_Vertices, m_Faces, m_Normals, texCoords, m_BoneIDs, m_Weights,
        "assets/shader/Model/vertex.shader",
        "assets/shader/Model/fragment.shader");

    Mesh<Types>::VertexBufferLayoutMap layoutMap;

    layoutMap["vertices"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLfloat>(3);
    };
    layoutMap["normals"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLfloat>(3);
    };
    layoutMap["texCoords"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLfloat>(2);
    };
    layoutMap["boneIDs"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLint>(MAX_BONE_INFLUENCE);
    };
    layoutMap["weights"] = [](std::shared_ptr<VertexBufferLayout> layout) {
        layout->push<GLfloat>(MAX_BONE_INFLUENCE);
    };

    m_Mesh->setup(layoutMap);
}

void ModelRender::loadTextures() {
    std::string diffusePath =
        "assets/model/Nightshade/textures/Nightshade_diffuse.png";
    std::string glowPath =
        "assets/model/Nightshade/textures/Nightshade_glow.png";
    std::string specularPath =
        "assets/model/Nightshade/textures/Nightshade_specular.png";
    std::string normalPath =
        "assets/model/Nightshade/textures/Nightshade_normal.png";

    auto diffuseTexture = std::make_shared<Texture>(diffusePath);
    auto glowTexture = std::make_shared<Texture>(glowPath);
    auto specularTexture = std::make_shared<Texture>(specularPath);
    auto normalTexture = std::make_shared<Texture>(normalPath);

    m_Mesh->setTexture(diffuseTexture);
    m_Mesh->setTexture(glowTexture);
    m_Mesh->setTexture(specularTexture);
    m_Mesh->setTexture(normalTexture);
}

void ModelRender::setUniforms() {
    Mesh<Types>::UniformsMap uniforms;

    uniforms["uTexture1"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uTexture1", 0);
    };
    uniforms["uTexture2"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uTexture2", 1);
    };
    uniforms["uTexture3"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uTexture3", 2);
    };
    uniforms["uTexture4"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uTexture4", 3);
    };
    uniforms["uLightPos"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform3f("uLightPos", glm::vec3(10.0f, 1.0f, 10.0f));
    };
    uniforms["uLightColor"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform3f("uLightColor", glm::vec3(1.0f));
    };
    uniforms["uCameraPosition"] = [this](std::shared_ptr<Shader> shader) {
        glm::vec3 cameraPos = glm::vec3(glm::inverse(VIEW_MATRIX)[3]);
        shader->setUniform3f("uCameraPosition", cameraPos);
    };

    m_Mesh->setUniforms(uniforms);
}

void ModelRender::setRunUniforms() {
    Mesh<Types>::UniformsMap uniforms;

    uniforms["uMVP"] = [this](std::shared_ptr<Shader> shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, m_Scale * 4.0f);
        model = glm::translate(model, m_Translation);

        glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), m_Rotation.x,
                                                glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), -m_Rotation.y,
                                                glm::vec3(1.0f, 0.0f, 0.0f));
        model *= rotationMatrixX * rotationMatrixY;
        m_ModelMatrix = model;

        glm::mat4 mvp = m_ProjMatrix * VIEW_MATRIX * model;
        shader->setUniformMat4f("uMVP", mvp);
    };
    uniforms["uFinalBonesMatrices"] = [this](std::shared_ptr<Shader> shader) {
        auto finalBoneMatrices = m_Animator->getFinalBoneMatrices();
        shader->setUniformMat4fv("uFinalBonesMatrices", finalBoneMatrices);
    };
    uniforms["uModel"] = [this](std::shared_ptr<Shader> shader) {
        shader->setUniformMat4f("uModel", m_ModelMatrix);
    };

    m_Mesh->setUniforms(uniforms);
}

void ModelRender::run() {
    if (!m_Mesh || !m_Animator)
        return;

    setRunUniforms();

    m_Mesh->draw();
}
