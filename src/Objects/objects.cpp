#include "objects.hpp"
#include "Common/cube.hpp"
#include "Common/plane.hpp"

Objects::Objects()
    : cylinder(std::make_shared<Cylinder>(2.0, 0.5, 0.5, 20)),
      sphere(std::make_shared<Sphere>(1.0, 20)), objectType(ObjectType::None),
      t(0.0f) {}

void Objects::update() {
    vertices.clear();
    indices.clear();

    switch (objectType) {
    case ObjectType::None:
        return;
        break;
    case ObjectType::Sphere:
        vertices = sphere->genVertices();
        indices = sphere->genIndices();
        break;
    case ObjectType::Cylinder:
        vertices = cylinder->genVertices();
        indices = cylinder->genIndices();
        break;
    case ObjectType::Plane:
        vertices = planeVertices;
        indices = planeIndices;
        break;
    case ObjectType::Cube:
        vertices = cubeVertices;
        indices = cubeIndices;
        break;
    }

    mesh = std::make_shared<Mesh<>>(vertices, indices,
                                    "assets/shader/Objects/vertex.shader",
                                    "assets/shader/Objects/fragment.shader");

    Mesh<>::VertexBufferLayoutMap layoutMap = {
        {"vertices",
         [](std::shared_ptr<VertexBufferLayout> layout) {
             layout->push<GLfloat>(3);
         }},
    };
    mesh->setup(layoutMap);
}

void Objects::setup() { update(); }

void Objects::run() {
    if (objectType == ObjectType::None) {
        return;
    }

    Mesh<>::UniformsMap uniforms;
    uniforms["uMVP"] = [this](std::shared_ptr<Shader> shader) {
        modelMat = glm::mat4(1.0f);
        glm::vec3 scale(3.0f);
        modelMat = glm::scale(modelMat, scale);

        glm::vec3 translation(3.0f, 0.0f, 0.0f);
        modelMat = glm::translate(modelMat, translation);

        float angle = t * glm::radians(90.0f);
        glm::mat4 rotationMatrix =
            glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));
        modelMat *= rotationMatrix;

        glm::mat4 mvp = projMat * viewMat * modelMat;
        shader->setUniformMat4f("uMVP", mvp);
    };
    uniforms["uCameraPosition"] = [this](std::shared_ptr<Shader> shader) {
        glm::vec3 cameraPos = glm::vec3(glm::inverse(viewMat)[3]);
        shader->setUniform3f("uCameraPosition", cameraPos);
    };
    uniforms["uObjectPosition"] = [this](std::shared_ptr<Shader> shader) {
        glm::vec3 objectPosition = glm::vec3(modelMat[3]);
        shader->setUniform3f("uObjectPosition", objectPosition);
    };

    mesh->setUniforms(uniforms);
    mesh->draw();
}
