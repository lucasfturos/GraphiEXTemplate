#pragma once

#include "imgui.h"

#include "Common/util.hpp"
#include "GLObjects/Mesh/mesh.hpp"
#include "GLObjects/framebuffer.hpp"
#include "Scenes/multi_scenes_option.hpp"

#include <memory>

enum class TransformMode { Rotate, Scale, Translate };

class ControlPanel {
  protected:
    const int m_ControlPanelWidth = 300;
    const int m_ControlPanelHeight = 130;

    const int m_GizmoWidth = 200;
    const int m_GizmoHeight = 200;

    const int m_GizmoControlWidth = 300;
    const int m_GizmoControlHeight = 280;

  private:
    int m_SceneOption;
    float m_ScaleFactor;
    bool m_ShowGizmoWindow;
    ObjectType m_ObjectType;
    TransformMode m_CurrentMode;

    glm::vec3 m_Scale;
    glm::vec2 m_Rotation;
    glm::vec3 m_Translation;

    std::shared_ptr<MultiScenesOption> m_MultiScenesOption;

  private:
    float m_BottomControlPanelHeight;
    std::shared_ptr<FrameBuffer> m_Framebuffer;
    std::shared_ptr<Mesh<MeshTypes<glm::vec3, GLuint>>> m_CubeMesh;

    void setupGizmo();

    // GUI
    void initFont();
    void styleWidget();
    void mainWindow();
    void gizmoWindow();

    // Window
    void setupGizmoWindow();
    void renderGizmoWindow();
    void renderGizmoControls();
    void renderSceneSelector();
    void resetTransformations();
    void renderObjectTypeSelector();
    void handleMouseInteraction(const ImVec2 &, const ImVec2 &);
    Mesh<MeshTypes<glm::vec3, GLuint>>::UniformsMap
    createUniforms(const ImVec2 &);

  public:
    ControlPanel(std::shared_ptr<MultiScenesOption>);

    ObjectType getObjectType() { return m_ObjectType; }

    glm::vec3 getScale() const { return m_Scale; }
    glm::vec2 getRotation() const { return m_Rotation; }
    glm::vec3 getTranslation() const { return m_Translation; }

    void setup();
    void run();
};
