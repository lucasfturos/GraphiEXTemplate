#pragma once

#include "Graphics/Mesh/mesh.hpp"
#include "Physics/softbody.hpp"
#include <glm/ext/matrix_transform.hpp>

class SoftBodyRender {
  protected:
    const glm::mat4 m_ViewMatrix =
        glm::lookAt(glm::vec3(0.0f, 2.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

  private:
    std::unique_ptr<SoftBody> m_SoftBody;

    glm::mat4 m_ProjMatrix;
    float m_Time;
    float m_SizeCornellBox;

  private:
    std::vector<std::shared_ptr<Mesh<Types>>> m_Mesh;

    glm::vec3 m_Scale;
    glm::vec2 m_Rotation;
    glm::vec3 m_Translation;

    void setupMesh();
    void setRunUniforms();
    void update();

  public:
    SoftBodyRender();

    void setTime(float dt) { m_Time += dt; }
    void setScale(glm::vec3 scale) { m_Scale = scale; }
    void setRotation(glm::vec2 rotation) { m_Rotation = rotation; }
    void setProjection(glm::mat4 projection) { m_ProjMatrix = projection; }
    void setTranslation(glm::vec3 translation) { m_Translation = translation; }

    void setup();
    void run();
};