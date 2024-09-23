#pragma once

#include "Common/cylinder.hpp"
#include "Common/sphere.hpp"
#include "Common/util.hpp"
#include "GLObjects/mesh.hpp"

#include <memory>

class Objects {
  protected:
    const glm::mat4 viewMat =
        glm::lookAt(glm::vec3(0.0f, 2.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 projMat =
        glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

  private:
    std::shared_ptr<Cylinder> cylinder;
    std::shared_ptr<Sphere> sphere;
    ObjectType objectType;

    float t;

  private:
    std::shared_ptr<Mesh<glm::vec3, glm::ivec3>> mesh;

    std::vector<GLuint> indices;
    std::vector<glm::vec3> vertices;

    void update();

  public:
    Objects();

    void setObjectType(ObjectType type) {
        if (objectType != type) {
            objectType = type;
            update();
        }
    }

    void setTime(float time) { t = time; }

    void setup();
    void run();
};