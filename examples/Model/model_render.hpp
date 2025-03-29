#pragma once

#include "Graphics/Animation/animator.hpp"
#include "Graphics/Mesh/mesh.hpp"
#include "Graphics/model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class ModelRender {
  protected:
    const glm::mat4 m_ViewMatrix =
        glm::lookAt({0.0f, 2.0f, 20.0f}, glm::vec3(0.0f), {0.0f, 1.0f, 0.0f});

  private:
    std::shared_ptr<Model> m_Model;
    std::shared_ptr<Mesh<Types>> m_Mesh;
    std::shared_ptr<Animation> m_Animation;
    std::shared_ptr<Animator> m_Animator;

    glm::mat4 m_ModelMatrix;
    glm::mat4 m_ProjMatrix;

  private:
    glm::vec3 m_Scale;
    glm::vec2 m_Rotation;
    glm::vec3 m_Translation;

    std::vector<GLuint> m_Faces;
    std::vector<glm::vec3> m_Vertices;
    std::vector<glm::vec3> m_Normals;
    std::vector<glm::ivec4> m_BoneIDs;
    std::vector<glm::vec4> m_Weights;

    void loadModel();
    void setupMesh();
    void loadTextures();
    void setUniforms();
    void setRunUniforms();

  public:
    ModelRender(const std::string &);

    void setDeltaTime(float dt) { m_Animator->update(dt); }

    void setScale(glm::vec3 scale) { m_Scale = scale; }
    void setRotation(glm::vec2 rotation) { m_Rotation = rotation; }
    void setProjection(glm::mat4 projection) { m_ProjMatrix = projection; }
    void setTranslation(glm::vec3 translation) { m_Translation = translation; }

    void setup();
    void run();
};
