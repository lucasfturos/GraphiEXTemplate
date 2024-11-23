#pragma once

#include "ControlPanel/control_panel.hpp"

#include <memory>

class Scene {
  protected:
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ModelMatrix;

    std::shared_ptr<ControlPanel> m_ControlPanel;

  public:
    Scene() = default;
    Scene(std::shared_ptr<ControlPanel> controlPanel)
        : m_ControlPanel(controlPanel) {}
    virtual ~Scene() = default;
    virtual void setup() = 0;
    virtual void update(float /* dt */, glm::mat4 /* projection */) = 0;
    virtual void render() = 0;
    virtual void resetMatrix() {
        m_ViewMatrix = glm::mat4(1.0f);
        m_ModelMatrix = glm::mat4(1.0f);
    }
};
