#pragma once

#include "ControlPanel/control_panel.hpp"
#include "Render/render.hpp"
#include "Volume/volume_obj.hpp"

class VolumetricScene : public Scene {
  private:
    std::shared_ptr<VolumeObject> m_VolumeObject;

  public:
    VolumetricScene(std::shared_ptr<ControlPanel> controlPanel,
                    std::shared_ptr<VolumeObject> volumeObject)
        : Scene(controlPanel), m_VolumeObject(volumeObject) {}

    void setup() override { m_VolumeObject->setup(); }

    void update(float /* dt */, glm::mat4 projection) override {
        m_VolumeObject->setProjection(projection);
        m_VolumeObject->setScale(m_ControlPanel->getScale());
        m_VolumeObject->setRotation(m_ControlPanel->getRotation());
        m_VolumeObject->setTranslation(m_ControlPanel->getTranslation());
    }

    void render() override { m_VolumeObject->run(); }
};
