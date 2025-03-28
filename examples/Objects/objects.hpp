#pragma once

#include "Common/Enums/object_type.hpp"
#include "Graphics/Mesh/mesh.hpp"
#include "Objects3D/Integrate.hpp"
#include "Objects3D/cylinder.hpp"
#include "Objects3D/sphere.hpp"

#include <glm/ext/matrix_transform.hpp>

class Objects {
  protected:
    const glm::mat4 m_ViewMatrix =
        glm::lookAt(glm::vec3(0.0f, 2.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

  private:
    std::shared_ptr<Cylinder> m_Cylinder;
    std::shared_ptr<Integrate> m_Integrate;
    std::shared_ptr<Sphere> m_Sphere;
    ObjectType m_ObjectType;

    float m_Time;

  private:
    glm::mat4 m_ProjMatrix;
    glm::mat4 m_ModelMatrix;
    std::shared_ptr<Mesh<Types>> m_Mesh;

    std::vector<GLuint> m_Indices;
    std::vector<glm::vec3> m_Vertices;

    void update();
    void updateObject();
    void createMesh();
    void setupModelMatrix();

  public:
    Objects();

    void setObjectType(ObjectType type) {
        if (m_ObjectType != type) {
            m_ObjectType = type;
            update();
        }
    }

    void setTime(float dt) { m_Time += dt; }
    void setProjection(glm::mat4 projection) { m_ProjMatrix = projection; }

    void setup() { update(); }
    void run();
};