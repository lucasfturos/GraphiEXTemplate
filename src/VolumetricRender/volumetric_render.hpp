#pragma once

#include "GLObjects/mesh.hpp"
#include "GLObjects/model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>

class VolumetricRender {
  protected:
    const glm::mat4 viewMat =
        glm::lookAt(glm::vec3(0.0f, 2.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

  private:
    std::shared_ptr<Model> model;

    glm::mat4 modelMat;
    glm::mat4 projMat;

  private:
    glm::vec3 scale;
    glm::vec2 rotation;
    glm::vec3 translation;

    std::vector<GLuint> modelFaces;
    std::vector<glm::vec3> modelVertices;

    std::shared_ptr<Mesh<>> mesh;

    void loadModel();
    void setupMesh();
    void loadTextures();
    void setUniforms();
    void setRunUniforms();
    std::vector<GLfloat> generateDensityData(int, int, int);

  public:
    VolumetricRender(const std::string &);

    void setScale(glm::vec3 s) { scale = s; }
    void setRotation(glm::vec2 rot) { rotation = rot; }
    void setTranslation(glm::vec3 trans) { translation = trans; }
    void setProjection(glm::mat4 projection) { projMat = projection; }

    void setup();
    void run();
};