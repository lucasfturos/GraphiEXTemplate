#include "objects.hpp"
#include "Common/cube.hpp"
#include "Common/plane.hpp"

Objects::Objects()
    : m_Cylinder(std::make_shared<Cylinder>(2.0, 0.5, 0.5, 20)),
      m_Sphere(std::make_shared<Sphere>(1.0, 20)),
      m_ObjectType(ObjectType::None), m_Time(0.0f) {}

void Objects::update() {
    updateVerticesAndIndices();
    createMesh();
}

void Objects::updateVerticesAndIndices() {
    m_Vertices.clear();
    m_Indices.clear();

    switch (m_ObjectType) {
    case ObjectType::None:
        return;
    case ObjectType::Sphere:
        m_Vertices = m_Sphere->genVertices();
        m_Indices = m_Sphere->genIndices();
        break;
    case ObjectType::Cylinder:
        m_Vertices = m_Cylinder->genVertices();
        m_Indices = m_Cylinder->genIndices();
        break;
    case ObjectType::Plane:
        m_Vertices = planeVertices;
        m_Indices = planeIndices;
        break;
    case ObjectType::Cube:
        m_Vertices = cubeVertices;
        m_Indices = cubeIndices;
        break;
    }
}

void Objects::createMesh() {
    m_Mesh = std::make_shared<Mesh<Types>>(
        m_Vertices, m_Indices, "assets/shader/Objects/vertex.shader",
        "assets/shader/Objects/fragment.shader");

    Mesh<Types>::VertexBufferLayoutMap layoutMap;
    layoutMap["vertices"] = &LayoutAttribute<GLfloat, 3>::setup;
    m_Mesh->setup(layoutMap);
}

void Objects::setupModelMatrix() {
    m_ModelMatrix = glm::mat4(1.0f);
    glm::vec3 scale(3.0f);
    m_ModelMatrix = glm::scale(m_ModelMatrix, scale);

    glm::vec3 translation(3.0f, 0.0f, 0.0f);
    m_ModelMatrix = glm::translate(m_ModelMatrix, translation);

    float angle = m_Time * glm::radians(90.0f);
    glm::mat4 rotationMatrix =
        glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));
    m_ModelMatrix *= rotationMatrix;
}

void Objects::run() {
    if (m_ObjectType == ObjectType::None) {
        return;
    }

    setupModelMatrix();

    Mesh<Types>::UniformsMap uniforms;
    uniforms["uMVP"] = [this](std::shared_ptr<Shader> shader) {
        glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * m_ModelMatrix;
        shader->setUniformMat4f("uMVP", mvp);
    };

    m_Mesh->setUniforms(uniforms);
    m_Mesh->draw();
}
