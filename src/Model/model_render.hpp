#pragma once

#include "GLObjects/mesh.hpp"
#include "GLObjects/model_loader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class ModelRender {
  private:
    std::shared_ptr<ModelLoader> modelLoader;
    std::shared_ptr<Mesh<glm::vec3, glm::ivec3>> mesh;

    glm::mat4 viewMat;
    glm::mat4 projMat;
    glm::mat4 modelMat;
    float t;

    std::vector<GLuint> indices;
    std::vector<glm::vec3> vertices;

  public:
    ModelRender(const std::string &filepath, glm::mat4 view,
                glm::mat4 projection);

    void setTime(float time) { t = time; }

    void setup();
    void run();
};
