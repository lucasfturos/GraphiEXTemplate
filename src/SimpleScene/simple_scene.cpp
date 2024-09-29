#include "simple_scene.hpp"

SimpleScene::SimpleScene(std::shared_ptr<ModelRender> modelRender,
                         std::shared_ptr<VolumetricRender> volumetricRender,
                         std::shared_ptr<Objects> objects,
                         std::shared_ptr<ControlPanel> controlPanel)
    : Scene(controlPanel), modelRender(modelRender),
      volumetricRender(volumetricRender), objects(objects) {}

void SimpleScene::setup() {
    // modelRender->setup();
    volumetricRender->setup();
    objects->setup();
}

void SimpleScene::update(float dt, glm::mat4 projection) {
    // modelRender->setDeltaTime(dt);
    // modelRender->setProjection(projection);
    // modelRender->setScale(controlPanel->getScale());
    // modelRender->setRotation(controlPanel->getRotation());
    // modelRender->setTranslation(controlPanel->getTranslation());

    volumetricRender->setProjection(projection);
    volumetricRender->setScale(controlPanel->getScale());
    volumetricRender->setRotation(controlPanel->getRotation());
    volumetricRender->setTranslation(controlPanel->getTranslation());

    objects->setTime(dt);
    objects->setProjection(projection);
    objects->setObjectType(controlPanel->getObjectType());
}

void SimpleScene::render() {
    // modelRender->run();
    volumetricRender->run();
    objects->run();
}