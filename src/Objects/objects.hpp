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

  private:
    std::shared_ptr<Cylinder> cylinder;
    std::shared_ptr<Sphere> sphere;
    ObjectType objectType;

    float t;

  private:
    glm::mat4 projMat;
    std::shared_ptr<Mesh<>> mesh;

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

    void setProjection(glm::mat4 projection) { projMat = projection; }

    void setup();
    void run();
};