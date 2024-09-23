#pragma once

#include "ControlPanel/control_panel.hpp"
#include "Model/model_render.hpp"
#include "Objects/objects.hpp"
#include "Render/scene.hpp"

class SimpleScene : public Scene {
  private:
    std::shared_ptr<ModelRender> modelRender;
    std::shared_ptr<Objects> objects;

  public:
    SimpleScene(std::shared_ptr<ModelRender> modelRender,
                std::shared_ptr<Objects> objects,
                std::shared_ptr<ControlPanel> controlPanel)
        : Scene(controlPanel), modelRender(modelRender), objects(objects) {}

    void setup() override {
        modelRender->setup();
        objects->setup();
    }

    void render(float time) override {
        modelRender->setRotation(controlPanel->getRotation());
        modelRender->run();

        objects->setTime(time);
        objects->setObjectType(controlPanel->getObjectType());
        objects->run();
    }
};
