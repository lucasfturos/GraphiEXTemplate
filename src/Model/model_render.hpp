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

  private:
    std::shared_ptr<ModelLoader> modelLoader;
    std::shared_ptr<Mesh<>> mesh;

    glm::mat4 modelMat;
    glm::mat4 projMat;
    
    glm::vec3 scale;
    glm::vec2 rotation;
    glm::vec3 translation;

    std::vector<GLuint> indices;
    std::vector<glm::vec3> vertices;

    void loadModel();
    void setupMesh();
    void loadTextures();
    void setUniforms();
    void setRunUniforms();
    glm::mat4 generateMVP();

  public:
    ModelRender(const std::string &filepath);

    void setScale(glm::vec3 s) { scale = s; }
    void setRotation(glm::vec2 rot) { rotation = rot; }
    void setTranslation(glm::vec3 trans) { translation = trans; }

    void setProjection(glm::mat4 projection) { projMat = projection; }

    void setup();
    void run();
};
