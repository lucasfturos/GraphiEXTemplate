#pragma once

#include "GLObjects/mesh.hpp"
#include "GLObjects/model_loader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class ModelRender {
  protected:
    const glm::mat4 viewMat =
        glm::lookAt(glm::vec3(0.0f, 2.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 projMat =
        glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

  private:
    std::shared_ptr<ModelLoader> modelLoader;
    std::shared_ptr<Mesh<glm::vec3, glm::ivec3>> mesh;

    glm::mat4 modelMat;
    glm::vec2 rotation;

    std::vector<GLuint> indices;
    std::vector<glm::vec3> vertices;

  public:
    ModelRender(const std::string &filepath);

    void setRotation(glm::vec2 rot) { rotation = rot; }

    void setup();
    void run();
};
