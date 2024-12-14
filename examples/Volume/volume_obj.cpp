#include "volume_obj.hpp"
#include "Objects3D/cube.hpp"

VolumeObject::VolumeObject(const std::string &filePath)
    : m_Model(std::make_shared<Model>(filePath)),
      m_ProjMatrix(
          glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f)) {}

void VolumeObject::setup() {
    loadModel();
    setupMesh();
    loadTextures();
    setUniforms();
}

void VolumeObject::loadModel() {
    m_ModelVertices = m_Model->getVertices();
    m_ModelFaces = m_Model->getFaces();
}

void VolumeObject::setupMesh() {
    m_Mesh = std::make_shared<Mesh<Types>>(
        cubeVertices, cubeIndices,
        ShaderPaths{"assets/shader/VolumeRender/vertex.shader"},
        ShaderPaths{"assets/shader/VolumeRender/fragment.shader"});

    Mesh<Types>::VertexBufferLayoutMap layoutMap;
    layoutMap["vertices"] = &LayoutAttribute<GLfloat, 3>::setup;
    m_Mesh->setup(layoutMap);
}

void VolumeObject::loadTextures() {
    int num = 69;
    int width = num;
    int height = num;
    int depth = num;

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

void VolumeObject::setUniforms() {
    Mesh<Types>::UniformsMap uniforms;

    uniforms["uVolume"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uVolume", 0);
    };
    uniforms["uTransferFunction"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uTransferFunction", 1);
    };
    uniforms["uCameraPosition"] = [this](std::shared_ptr<Shader> shader) {
        glm::vec3 cameraPos = glm::vec3(glm::inverse(m_ViewMatrix)[3]);
        shader->setUniform3f("uCameraPosition", cameraPos);
    };

    m_Mesh->setUniforms(uniforms);
}

void VolumeObject::setRunUniforms() {
    Mesh<Types>::UniformsMap uniforms;

    uniforms["uMVP"] = [this](std::shared_ptr<Shader> shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, m_Scale * 5.0f);

        glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), m_Rotation.x,
                                                glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), -m_Rotation.y,
                                                glm::vec3(1.0f, 0.0f, 0.0f));
        model *= rotationMatrixX * rotationMatrixY;
        m_ModelMatrix = model;

        glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model;
        shader->setUniformMat4f("uMVP", mvp);
    };
    uniforms["uModel"] = [this](std::shared_ptr<Shader> shader) {
        shader->setUniformMat4f("uModel", m_ModelMatrix);
    };
    uniforms["uTranslation"] = [this](std::shared_ptr<Shader> shader) {
        shader->setUniform3f("uTranslation", m_Translation);
    };

    m_Mesh->setUniforms(uniforms);
}

void VolumeObject::run() {
    if (!m_Mesh)
        return;

    setRunUniforms();

    m_Mesh->draw();
}