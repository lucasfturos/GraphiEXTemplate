#pragma once

#include "ControlPanel/control_panel.hpp"

#include <memory>

class Scene {
  protected:
    std::shared_ptr<ControlPanel> m_ControlPanel;

  public:
    Scene(std::shared_ptr<ControlPanel> controlPanel)
        : m_ControlPanel(controlPanel) {}
    virtual ~Scene() = default;
    virtual void setup() = 0;
    virtual void update(float /* deltaTime */, glm::mat4 /* projMat */) = 0;
    virtual void render() = 0;
};
