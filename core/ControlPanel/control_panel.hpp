#pragma once

#include "imgui.h"

#include "Common/util.hpp"
#include "GLObjects/Mesh/mesh.hpp"
#include "GLObjects/framebuffer.hpp"
#include "Render/multi_scenes_option.hpp"

#include <memory>

enum class TransformMode { Rotate, Scale, Translate };

class ControlPanel {
  protected:
    const int controlPanelWidth = 300;
    const int controlPanelHeight = 130;

    const int gizmoWidth = 200;
    const int gizmoHeight = 200;

    const int gizmoControlWidth = 300;
    const int gizmoControlHeight = 280;

  private:
    int sceneOption;
    float scaleFactor;
    bool showGizmoWindow;
    ObjectType objectType;
    TransformMode currentMode;

    glm::vec3 scale;
    glm::vec2 rotation;
    glm::vec3 translation;

    std::shared_ptr<MultiScenesOption> m_MultiScenesOption;

  private:
    float bottomControlPanelHeight;
    std::shared_ptr<FrameBuffer> framebuffer;
    std::shared_ptr<Mesh<MeshTypes<glm::vec3, GLuint>>> cubeMesh;

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

    ObjectType getObjectType() { return objectType; }

    glm::vec3 getScale() const { return scale; }
    glm::vec2 getRotation() const { return rotation; }
    glm::vec3 getTranslation() const { return translation; }

    void setup();
    void run();
};
