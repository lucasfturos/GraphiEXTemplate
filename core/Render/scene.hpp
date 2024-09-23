#pragma once

#include "ControlPanel/control_panel.hpp"

#include <memory>

class Scene {
  protected:
    std::shared_ptr<ControlPanel> controlPanel;

  public:
    Scene(std::shared_ptr<ControlPanel> controlPanel)
        : controlPanel(controlPanel) {}
    virtual ~Scene() = default;
    virtual void setup() = 0;
    virtual void update(float /* deltaTime */) = 0;
    virtual void render() = 0;
};
