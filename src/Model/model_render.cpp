#include "model_render.hpp"
#include <iostream>

ModelRender::ModelRender(const std::string &filepath, glm::mat4 view,
                         glm::mat4 projection)
    : modelLoader(std::make_shared<ModelLoader>(filepath)), viewMat(view),
      projMat(projection), modelMat(glm::mat4(1.0f)) {}

void ModelRender::setup() {
    vertices = modelLoader->getVertices();
    indices = modelLoader->getFaces();

    mesh = std::make_shared<Mesh<glm::vec3, glm::ivec3>>(
        vertices, indices, "assets/shader/Model/vertex.shader",
        "assets/shader/Model/fragment.shader");
    mesh->setup<GLfloat>({3});
}

void ModelRender::run() {
    if (!mesh) {
        return;
    }

    Mesh<glm::vec3, glm::ivec3>::UniformsMap uniforms = {
        {"uMVP",
         [this](std::shared_ptr<Shader> shader) {
             glm::mat4 model = glm::mat4(1.0f);

             glm::vec3 scale(5.0f);
             model = glm::scale(model, scale);

             glm::mat4 rotationMatrixX = glm::rotate(
                 glm::mat4(1.0f), rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
             glm::mat4 rotationMatrixY = glm::rotate(
                 glm::mat4(1.0f), -rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
             model *= rotationMatrixX * rotationMatrixY;

             glm::mat4 mvp = projMat * viewMat * model;
             shader->setUniformMat4f("uMVP", mvp);
         }},
    };

    mesh->setUniforms(uniforms);
    mesh->draw();
}
