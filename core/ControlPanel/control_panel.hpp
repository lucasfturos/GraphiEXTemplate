#pragma once

#include "GLObjects/framebuffer.hpp"
#include "imgui.h"

#include "Common/util.hpp"
#include "GLObjects/mesh.hpp"

#include <memory>

class ControlPanel {
  protected:
    const int windowWidth = 300;
    const int windowHeight = 300;
    const int gizmoWidth = 200;
    const int gizmoHeight = 200;

  private:
    ObjectType objectType;
    bool showGizmoWindow;

    glm::vec2 rotation;
    std::shared_ptr<FrameBuffer> framebuffer;

  private:
    std::shared_ptr<Mesh<glm::vec3, GLuint>> cubeMesh;

    void setupGizmo();

    // GUI
    void initFont();
    void styleWidget();
    void mainWindow();
    void gizmoWindow();

  public:
    ControlPanel();

    ObjectType getObjectType() { return objectType; }
    glm::vec2 getRotation() const { return rotation; }
    
    void setup();
    void run();
};
