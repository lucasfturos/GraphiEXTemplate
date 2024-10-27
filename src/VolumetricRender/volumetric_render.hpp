#pragma once

#include "GLObjects/Mesh/mesh.hpp"
#include "GLObjects/model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class VolumetricRender {
  protected:
    const glm::mat4 m_ViewMatrix =
        glm::lookAt(glm::vec3(0.0f, 2.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

  private:
    std::shared_ptr<Model> m_Model;

    glm::mat4 m_ModelMatrix;
    glm::mat4 m_ProjMatrix;

  private:
    glm::vec3 m_Scale;
    glm::vec2 m_Rotation;
    glm::vec3 m_Translation;

    glm::vec3 m_VolumeScale;
    glm::ivec3 m_VolumeDimension;

    std::vector<GLuint> m_ModelFaces;
    std::vector<glm::vec3> m_ModelVertices;

    std::shared_ptr<Mesh<Types>> m_Mesh;

    void loadModel();
    void setupMesh();
    void loadTextures();
    void setUniforms();
    void setRunUniforms();

  public:
    VolumetricRender(const std::string &);

    void setScale(glm::vec3 scale) { m_Scale = scale; }
    void setRotation(glm::vec2 rotation) { m_Rotation = rotation; }
    void setProjection(glm::mat4 projection) { m_ProjMatrix = projection; }
    void setTranslation(glm::vec3 translation) { m_Translation = translation; }

    void setup();
    void run();
};