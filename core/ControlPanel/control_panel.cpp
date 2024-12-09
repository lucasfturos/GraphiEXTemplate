#include "control_panel.hpp"
#include "Objects3D/cube.hpp"

ControlPanel::ControlPanel(std::shared_ptr<MultiScenesOption> multiScenesOption)
    : m_ScaleFactor(1.0f), m_ShowGizmoWindow(true),
      m_ObjectType(ObjectType::None), m_CurrentMode(TransformMode::Rotate),
      m_Scale(1.0f), m_Rotation(0.0f), m_Translation(0.0f),
      m_MultiScenesOption(multiScenesOption) {}

void ControlPanel::setup() {
    styleWidget();
    initFont();
    setupGizmo();
}

void ControlPanel::setupGizmo() {
    m_CubeMesh = std::make_shared<Mesh<MeshTypes<glm::vec3, GLuint>>>(
        cubeVertices, cubeIndices, "assets/shader/Gizmo/vertex.shader",
        "assets/shader/Gizmo/fragment.shader");

    Mesh<MeshTypes<glm::vec3, glm::ivec3>>::VertexBufferLayoutMap layoutMap;
    layoutMap["vertices"] = &LayoutAttribute<GLfloat, 3>::setup;

    m_CubeMesh->setup(layoutMap);

    m_Framebuffer = std::make_shared<FrameBuffer>(m_GizmoWidth, m_GizmoHeight);
}

void ControlPanel::run() {
    mainWindow();
    gizmoWindow();
}
