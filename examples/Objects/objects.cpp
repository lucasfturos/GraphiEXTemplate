#include "objects.hpp"
#include "Common/util.hpp"
#include "Objects3D/cube.hpp"
#include "Objects3D/plane.hpp"

inline std::vector<std::function<GLfloat(GLfloat, GLfloat)>> functions = {
    [](GLfloat u, GLfloat v) {
        return (1.0 + 0.5 * std::cos(v)) * std::cos(u);
    },
    [](GLfloat u, GLfloat v) {
        return (1.0 + 0.5 * std::cos(v)) * std::sin(u);
    },
    [](GLfloat, GLfloat v) { return 0.5 * std::sin(v); },
};

Objects::Objects()
    : m_Cylinder(std::make_shared<Cylinder>(2.0, 0.5, 0.5, 20)),
      m_Integrate(std::make_shared<Integrate>(functions, 0, 2 * M_PI, 0,
                                              2 * M_PI, 50, 30)),
      m_Sphere(std::make_shared<Sphere>(1.0, 20)),
      m_ObjectType(ObjectType::None), m_Time(0.0f) {}

void Objects::update() {
    updateObject();
    createMesh();
}

void Objects::updateObject() {
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
    case ObjectType::FunctionPlot:
        m_Vertices = m_Integrate->getVertices();
        m_Indices = m_Integrate->getFaces();
        break;
    }
}

void Objects::createMesh() {
    m_Mesh = std::make_shared<Mesh<Types>>(
        m_Vertices, m_Indices,
        ShaderPaths{"assets/shader/Objects/vertex.shader"},
        ShaderPaths{"assets/shader/Objects/fragment.shader"});

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
