#pragma once

#include "ControlPanel/control_panel.hpp"
#include "Model/model_render.hpp"
#include "Objects/objects.hpp"
#include "Render/render.hpp"

class SimpleScene : public Scene {
  private:
    std::shared_ptr<ModelRender> m_ModelRender;
    std::shared_ptr<Objects> m_Objects;

  public:
    SimpleScene(std::shared_ptr<ControlPanel> controlPanel,
                std::shared_ptr<ModelRender> modelRender,
                std::shared_ptr<Objects> objects)
        : Scene(controlPanel), m_ModelRender(modelRender), m_Objects(objects) {}

    void setup() override {
        m_ModelRender->setup();
        m_Objects->setup();
    }

    void update(float dt, glm::mat4 projection) override {
        m_ModelRender->setDeltaTime(dt);
        m_ModelRender->setProjection(projection);
        m_ModelRender->setScale(m_ControlPanel->getScale());
        m_ModelRender->setRotation(m_ControlPanel->getRotation());
        m_ModelRender->setTranslation(m_ControlPanel->getTranslation());

        m_Objects->setTime(dt);
        m_Objects->setProjection(projection);
        m_Objects->setObjectType(m_ControlPanel->getObjectType());
    }

    void render() override {
        m_ModelRender->run();
        m_Objects->run();
    }
};
