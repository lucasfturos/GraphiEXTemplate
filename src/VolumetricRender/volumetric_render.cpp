#include "volumetric_render.hpp"
#include "Common/cube.hpp"

VolumetricRender::VolumetricRender(const std::string &filePath)
    : m_Model(std::make_shared<Model>(filePath)),
      m_ProjMatrix(
          glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f)) {}

void VolumetricRender::setup() {
    loadModel();
    setupMesh();
    loadTextures();
    setUniforms();
}

void VolumetricRender::loadModel() {
    m_ModelVertices = m_Model->getVertices();
    m_ModelFaces = m_Model->getFaces();
}

void VolumetricRender::setupMesh() {
    m_Mesh = std::make_shared<Mesh<Types>>(
        cubeVertices, cubeIndices, "assets/shader/VolumeRender/vertex.shader",
        "assets/shader/VolumeRender/fragment.shader");

    Mesh<Types>::VertexBufferLayoutMap layoutMap;
    layoutMap["vertices"] = &LayoutAttribute<GLfloat, 3>::setup;
    m_Mesh->setup(layoutMap);
}

void VolumetricRender::loadTextures() {
    int num = 2;
    int width = num;
    int height = num;
    int depth = num;

    m_VolumeDimension = glm::ivec3(width, height, depth);
    int longestAxis = std::max(width, std::max(height, depth));
    m_VolumeScale = glm::vec3(static_cast<float>(width) / longestAxis,
                              static_cast<float>(height) / longestAxis,
                              static_cast<float>(depth) / longestAxis);

    std::vector<GLfloat> densityData = VolumeGeneration::generateDensityData(
        m_ModelVertices, m_ModelFaces, width, height, depth);
    auto modelTexture = std::make_shared<Texture>(width, height, depth, GL_RED,
                                                  GL_FLOAT, GL_TEXTURE_3D);

    modelTexture->updateData(densityData, width, height, depth, GL_RED,
                             GL_FLOAT);

    int tfWidth = 256;
    std::vector<GLfloat> transferFunctionData =
        VolumeGeneration::generateTransferFunction(tfWidth);
    auto transferFunctionTexture = std::make_shared<Texture>(
        tfWidth, 1, 1, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D);
    transferFunctionTexture->updateData(transferFunctionData, tfWidth, 1, 1,
                                        GL_RGBA, GL_FLOAT);

    m_Mesh->setTexture(modelTexture);
    m_Mesh->setTexture(transferFunctionTexture);
}

void VolumetricRender::setUniforms() {
    Mesh<Types>::UniformsMap uniforms;

    uniforms["uModel"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uModel", 0);
    };
    uniforms["uCameraPosition"] = [this](std::shared_ptr<Shader> shader) {
        glm::vec3 cameraPos = glm::vec3(glm::inverse(m_ViewMatrix)[3]);
        shader->setUniform3f("uCameraPosition", cameraPos);
    };
    uniforms["uTransferFunction"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uTransferFunction", 1);
    };
    uniforms["uVolumeScale"] = [this](std::shared_ptr<Shader> shader) {
        shader->setUniform3f("uVolumeScale", m_VolumeScale);
    };
    uniforms["uVolumeDimension"] = [this](std::shared_ptr<Shader> shader) {
        shader->setUniform3f("uVolumeDimension", m_VolumeDimension);
    };
    uniforms["uDtScale"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1f("uDtScale", 0.1f);
    };

    m_Mesh->setUniforms(uniforms);
}

void VolumetricRender::setRunUniforms() {
    Mesh<Types>::UniformsMap uniforms;

    uniforms["uMVP"] = [this](std::shared_ptr<Shader> shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, m_Scale * 5.0f);
        model = glm::translate(model, m_Translation);

        glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), m_Rotation.x,
                                                glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), -m_Rotation.y,
                                                glm::vec3(1.0f, 0.0f, 0.0f));
        model *= rotationMatrixX * rotationMatrixY;

        glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model;
        shader->setUniformMat4f("uMVP", mvp);
    };

    m_Mesh->setUniforms(uniforms);
}

void VolumetricRender::run() {
    if (!m_Mesh)
        return;

    setRunUniforms();

    m_Mesh->draw();
}