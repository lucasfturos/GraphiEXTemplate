#pragma once

#include "GLObjects/animation.hpp"
#include "GLObjects/animator.hpp"
#include "GLObjects/mesh.hpp"
#include "GLObjects/model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class ModelRender {
  protected:
    const glm::mat4 viewMat =
        glm::lookAt(glm::vec3(0.0f, 2.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

  private:
    std::shared_ptr<Model> model;
    std::shared_ptr<Mesh<>> mesh;
    std::shared_ptr<Animation> animation;
    std::shared_ptr<Animator> animator;

    glm::mat4 modelMat;
    glm::mat4 projMat;

  private:
    glm::vec3 scale;
    glm::vec2 rotation;
    glm::vec3 translation;

    std::vector<GLuint> faces;
    std::vector<glm::vec3> vertices;
    std::vector<GLint> boneIDs;
    std::vector<GLuint> weights;

    void loadModel();
    void setupMesh();
    void loadTextures();
    void setUniforms();
    void setRunUniforms();

  public:
    ModelRender(const std::string &filepath);

    void setDeltaTime(float dt) { animator->update(dt); }

    void setScale(glm::vec3 s) { scale = s; }
    void setRotation(glm::vec2 rot) { rotation = rot; }
    void setTranslation(glm::vec3 trans) { translation = trans; }
    void setProjection(glm::mat4 projection) { projMat = projection; }

    void setup();
    void run();
};
