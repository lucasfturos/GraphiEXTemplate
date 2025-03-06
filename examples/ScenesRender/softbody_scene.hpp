#pragma once

#include "Scenes/scene.hpp"
#include "SoftBodyRender/soft_body_render.hpp"
#include <memory>

class SoftBodyScene : public Scene {
  private:
    std::shared_ptr<SoftBodyRender> m_SoftBodyRender;

  public:
    SoftBodyScene(std::shared_ptr<ControlPanel> controlPanel)
        : Scene(controlPanel),
          m_SoftBodyRender(std::make_shared<SoftBodyRender>()) {}

    void setup() override { m_SoftBodyRender->setup(); }

    void update(float dt, glm::mat4 projection) override {
        m_SoftBodyRender->setTime(dt);
        m_SoftBodyRender->setProjection(projection);
        m_SoftBodyRender->setScale(m_ControlPanel->getScale());
        m_SoftBodyRender->setRotation(m_ControlPanel->getRotation());
        m_SoftBodyRender->setTranslation(m_ControlPanel->getTranslation());
    }

    void render() override { m_SoftBodyRender->run(); }
};