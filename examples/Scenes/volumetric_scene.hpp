#pragma once

#include "ControlPanel/control_panel.hpp"
#include "Render/render.hpp"
#include "VolumetricRender/volumetric_render.hpp"

class VolumetricScene : public Scene {
  private:
    std::shared_ptr<VolumetricRender> m_VolumetricRender;

  public:
    VolumetricScene(std::shared_ptr<ControlPanel> controlPanel,
                    std::shared_ptr<VolumetricRender> volumetricRender)
        : Scene(controlPanel), m_VolumetricRender(volumetricRender) {}

    void setup() override { m_VolumetricRender->setup(); }

    void update(float /* dt */, glm::mat4 projection) override {
        m_VolumetricRender->setProjection(projection);
        m_VolumetricRender->setScale(m_ControlPanel->getScale());
        m_VolumetricRender->setRotation(m_ControlPanel->getRotation());
        m_VolumetricRender->setTranslation(m_ControlPanel->getTranslation());
    }

    void render() override { m_VolumetricRender->run(); }
};
