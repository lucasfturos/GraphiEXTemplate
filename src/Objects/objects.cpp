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

    Mesh<>::UniformsMap uniforms = {
        {"uMVP",
         [this](std::shared_ptr<Shader> shader) {
             glm::mat4 model = glm::mat4(1.0f);

             glm::vec3 scale(3.0f);
             model = glm::scale(model, scale);

             glm::vec3 translation(3.0f, 0.0f, 0.0f);
             model = glm::translate(model, translation);

             float angle = t * glm::radians(90.0f);
             glm::mat4 rotationMatrix = glm::rotate(
                 glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));
             model *= rotationMatrix;

             glm::mat4 mvp = projMat * viewMat * model;
             shader->setUniformMat4f("uMVP", mvp);
         }},
    };

    mesh->setUniforms(uniforms);
    mesh->draw();
}
