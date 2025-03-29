#include "soft_body_render.hpp"
#include "Objects3D/cube.hpp"
#include "Objects3D/plane.hpp"
#include <glm/ext/matrix_clip_space.hpp>

SoftBodyRender::SoftBodyRender()
    : m_SoftBody(std::make_unique<SoftBody>()),
      m_ProjMatrix(
          glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f)),
      m_Time(0.0f), m_SizeCornellBox(7.0f) {}

void SoftBodyRender::setup() {
    setupMesh();
    update();
}

void SoftBodyRender::setupMesh() {
    m_Mesh = {
        // Soft-body
        std::make_shared<Mesh<Types>>(
            cubeVertices, cubeIndices,
            ShaderPaths{"assets/shader/SoftBody/Cube/vertex.shader"},
            ShaderPaths{"assets/shader/SoftBody/Cube/fragment.shader"}),
        // Left Wall (Red)
        std::make_shared<Mesh<Types>>(
            planeVertices, planeIndices,
            ShaderPaths{
                "assets/shader/SoftBody/CornellBox/vertex.shader"}, // Vertex
            ShaderPaths{
                "assets/shader/SoftBody/CornellBox/red.shader"}), // Frag
        // Right Wall (Green)
        std::make_shared<Mesh<Types>>(
            planeVertices, planeIndices,
            ShaderPaths{"assets/shader/SoftBody/CornellBox/vertex.shader"},
            ShaderPaths{"assets/shader/SoftBody/CornellBox/green.shader"}),
        // Back Wall (White)
        std::make_shared<Mesh<Types>>(
            planeVertices, planeIndices,
            ShaderPaths{"assets/shader/SoftBody/CornellBox/vertex.shader"},
            ShaderPaths{"assets/shader/SoftBody/CornellBox/white.shader"}),
        // // Front Wall (White)
        // std::make_shared<Mesh<Types>>(
        //     planeVertices, planeIndices,
        //     ShaderPaths{"assets/shader/SoftBody/CornellBox/vertex.shader"},
        //     ShaderPaths{"assets/shader/SoftBody/CornellBox/white.shader"}),
        // Ceiling (White)
        std::make_shared<Mesh<Types>>(
            planeVertices, planeIndices,
            ShaderPaths{"assets/shader/SoftBody/CornellBox/vertex.shader"},
            ShaderPaths{"assets/shader/SoftBody/CornellBox/white.shader"}),
        // Floor (White)
        std::make_shared<Mesh<Types>>(
            planeVertices, planeIndices,
            ShaderPaths{"assets/shader/SoftBody/CornellBox/vertex.shader"},
            ShaderPaths{"assets/shader/SoftBody/CornellBox/white.shader"}),
    };

    Mesh<Types>::VertexBufferLayoutMap layoutMap;
    layoutMap["vertices"] = &LayoutAttribute<GLfloat, 3>::setup;

    for (auto &mesh : m_Mesh) {
        if (mesh)
            mesh->setup(layoutMap);
    }

    for (const auto &vertex : cubeVertices) {
        m_SoftBody->addParticle(vertex);
    }

    for (std::size_t i = 0; i < cubeIndices.size() - 1; ++i) {
        m_SoftBody->addSpring(cubeIndices[i], cubeIndices[i + 1]);
    }

    m_SoftBody->setBoundingBox(glm::vec3(-m_SizeCornellBox),
                               glm::vec3(m_SizeCornellBox));
}

void SoftBodyRender::setRunUniforms() {
    Mesh<Types>::UniformsMap uniformsBodySoft;

    uniformsBodySoft["uMVP"] = [this](std::shared_ptr<Shader> shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, m_Scale);
        model = glm::translate(model, m_Translation);

        glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), m_Rotation.x,
                                                glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), -m_Rotation.y,
                                                glm::vec3(1.0f, 0.0f, 0.0f));
        model *= rotationMatrixX * rotationMatrixY;

        glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model;
        shader->setUniformMat4f("uMVP", mvp);
    };

    m_Mesh[0]->setUniforms(uniformsBodySoft);

    Mesh<Types>::UniformsMap uniformsCornellBox;

    const std::vector<glm::mat4> transformations = {
        // Left Wall (Red)
        glm::rotate(
            glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
            glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        // Right Wall (Green)
        glm::rotate(
            glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        // Back Wall (White)
        glm::rotate(
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // // Front Wall (White)
        // glm::rotate(
        //     glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        //     glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // Ceiling (White)
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        // Floor (White)
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
    };

    for (std::size_t i = 0; i < transformations.size(); ++i) {
        uniformsCornellBox["uMVP"] = [this, i, &transformations](
                                         std::shared_ptr<Shader> shader) {
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 rotationMatrixX = glm::rotate(
                glm::mat4(1.0f), m_Rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 rotationMatrixY = glm::rotate(
                glm::mat4(1.0f), -m_Rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
            model *= rotationMatrixX * rotationMatrixY;

            model = glm::scale(model, m_Scale * m_SizeCornellBox);
            model *= transformations[i];

            glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model;
            shader->setUniformMat4f("uMVP", mvp);
        };
        m_Mesh[i + 1]->setUniforms(uniformsCornellBox);
    }
}

void SoftBodyRender::update() {
    m_SoftBody->setTimeStep(0.01f);
    m_SoftBody->update();
    std::vector<glm::vec3> newVertices = m_SoftBody->getUpdatedVertices();
    m_Mesh[0]->updateVertices(newVertices);
}

void SoftBodyRender::run() {
    if (m_Mesh.empty())
        return;

    update();
    setRunUniforms();

    for (const auto &mesh : m_Mesh) {
        if (!mesh)
            return;
        mesh->draw();
    }
}
