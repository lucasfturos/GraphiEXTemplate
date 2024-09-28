#include "simple_scene.hpp"

SimpleScene::SimpleScene(std::shared_ptr<ModelRender> modelRender,
                         std::shared_ptr<Objects> objects,
                         std::shared_ptr<ControlPanel> controlPanel)
    : Scene(controlPanel), modelRender(modelRender), objects(objects) {}

void SimpleScene::setup() {
    modelRender->setup();
    objects->setup();
}

void SimpleScene::update(float time, glm::mat4 projection) {
    modelRender->setTime(time);
    modelRender->setScale(controlPanel->getScale());
    modelRender->setRotation(controlPanel->getRotation());
    modelRender->setTranslation(controlPanel->getTranslation());

    objects->setTime(time);
    objects->setProjection(projection);
    objects->setObjectType(controlPanel->getObjectType());
}

void SimpleScene::render() {
    modelRender->run();
    objects->run();
}