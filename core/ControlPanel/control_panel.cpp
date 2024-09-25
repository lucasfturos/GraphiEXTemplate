#include "control_panel.hpp"
#include "Common/cube.hpp"

ControlPanel::ControlPanel()
    : objectType(ObjectType::None), showGizmoWindow(true), rotation(0.0f) {}

void ControlPanel::setup() {
    styleWidget();
    initFont();
    setupGizmo();
}

void ControlPanel::setupGizmo() {
    cubeMesh = std::make_shared<Mesh<glm::vec3, GLuint>>(
        cubeVertices, cubeIndices, "assets/shader/Gizmo/vertex.shader",
        "assets/shader/Gizmo/fragment.shader");

    Mesh<glm::vec3, glm::ivec3>::VertexBufferLayoutMap layoutMap = {
        {"vertices",
         [](std::shared_ptr<VertexBufferLayout> layout) {
             layout->push<GLfloat>(3);
         }},
    };
    cubeMesh->setup(layoutMap);
    
    framebuffer = std::make_shared<FrameBuffer>(gizmoWidth, gizmoHeight);
}

void ControlPanel::run() {
    mainWindow();
    gizmoWindow();
}
