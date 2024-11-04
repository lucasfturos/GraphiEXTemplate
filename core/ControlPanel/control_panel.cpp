#include "control_panel.hpp"
#include "Common/cube.hpp"

ControlPanel::ControlPanel(std::shared_ptr<MultiScenesOption> multiScenesOption)
    : scaleFactor(1.0f), showGizmoWindow(true), objectType(ObjectType::None),
      currentMode(TransformMode::Rotate), scale(1.0f), rotation(0.0f),
      translation(0.0f), m_MultiScenesOption(multiScenesOption) {}

void ControlPanel::setup() {
    styleWidget();
    initFont();
    setupGizmo();
}

void ControlPanel::setupGizmo() {
    cubeMesh = std::make_shared<Mesh<MeshTypes<glm::vec3, GLuint>>>(
        cubeVertices, cubeIndices, "assets/shader/Gizmo/vertex.shader",
        "assets/shader/Gizmo/fragment.shader");

    Mesh<MeshTypes<glm::vec3, glm::ivec3>>::VertexBufferLayoutMap layoutMap;
    layoutMap["vertices"] = &LayoutAttribute<GLfloat, 3>::setup;

    cubeMesh->setup(layoutMap);

    framebuffer = std::make_shared<FrameBuffer>(gizmoWidth, gizmoHeight);
}

void ControlPanel::run() {
    mainWindow();
    gizmoWindow();
}
