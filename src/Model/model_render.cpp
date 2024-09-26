#include "model_render.hpp"

ModelRender::ModelRender(const std::string &filepath)
    : modelLoader(std::make_shared<ModelLoader>(filepath)),
      modelMat(glm::mat4(1.0f)),
      projMat(
          glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f)) {}

void ModelRender::setup() {
    loadModel();
    setupMesh();
    loadTextures();
    setUniforms();
}

void ModelRender::loadModel() {
    vertices = modelLoader->getVertices();
    indices = modelLoader->getFaces();
}

void ModelRender::setupMesh() {
    auto texCoords = modelLoader->getTexCoords();
    mesh = std::make_shared<Mesh<>>(vertices, indices, texCoords,
                                    "assets/shader/Model/vertex.shader",
                                    "assets/shader/Model/fragment.shader");

    Mesh<>::VertexBufferLayoutMap layoutMap = {
        {"vertices",
         [](std::shared_ptr<VertexBufferLayout> layout) {
             layout->push<GLfloat>(3);
         }},
        {"texCoords",
         [](std::shared_ptr<VertexBufferLayout> layout) {
             layout->push<GLfloat>(2);
         }},
    };

    mesh->setup(layoutMap);
}

void ModelRender::loadTextures() {
    std::string diffusePath =
        "assets/model/Nightshade/textures/Nightshade_diffuse.png";
    std::string glowPath =
        "assets/model/Nightshade/textures/Nightshade_glow.png";
    std::string specularPath =
        "assets/model/Nightshade/textures/Nightshade_specular.png";

    auto diffuseTexture = std::make_shared<Texture>(diffusePath);
    auto glowTexture = std::make_shared<Texture>(glowPath);
    auto specularTexture = std::make_shared<Texture>(specularPath);

    mesh->setTexture(diffuseTexture);
    mesh->setTexture(glowTexture);
    mesh->setTexture(specularTexture);
}

void ModelRender::setUniforms() {
    Mesh<>::UniformsMap uniforms = {
        {"texture1",
         [](std::shared_ptr<Shader> shader) {
             shader->setUniform1i("texture1", 0);
         }},
        {"texture2",
         [](std::shared_ptr<Shader> shader) {
             shader->setUniform1i("texture2", 1);
         }},
        {"texture3",
         [](std::shared_ptr<Shader> shader) {
             shader->setUniform1i("texture3", 2);
         }},
    };
    mesh->setUniforms(uniforms);
}

glm::mat4 ModelRender::generateMVP() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, (scale == glm::vec3(1.0)) ? glm::vec3(0.07)
                                                        : scale); // Debug
    model = glm::translate(model, (translation == glm::vec3(0.0f))
                                      ? glm::vec3(0, -110.0, 0)
                                      : translation); // Debug

    glm::mat4 rotationMatrixX =
        glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationMatrixY =
        glm::rotate(glm::mat4(1.0f), -rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    model *= rotationMatrixX * rotationMatrixY;

    return projMat * viewMat * model;
}

void ModelRender::setRunUniforms() {
    Mesh<>::UniformsMap uniforms = {
        {"uMVP",
         [this](std::shared_ptr<Shader> shader) {
             glm::mat4 mvp = generateMVP();
             shader->setUniformMat4f("uMVP", mvp);
         }},
    };

    mesh->setUniforms(uniforms);
}

void ModelRender::run() {
    if (!mesh) {
        return;
    }

    setRunUniforms();

    mesh->getTexture(0)->bind(0);
    mesh->getTexture(1)->bind(1);
    mesh->getTexture(2)->bind(2);

    mesh->draw();
}
