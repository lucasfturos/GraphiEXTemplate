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
    objects->setTime(time);
    objects->setProjection(projection);
}

void SimpleScene::render() {
    modelRender->setRotation(controlPanel->getRotation());
    modelRender->run();

    objects->setObjectType(controlPanel->getObjectType());
    objects->run();
}