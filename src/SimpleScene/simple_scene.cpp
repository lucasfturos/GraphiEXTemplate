#include "simple_scene.hpp"

SimpleScene::SimpleScene(std::shared_ptr<ModelRender> modelRender,
                         std::shared_ptr<VolumetricRender> volumetricRender,
                         std::shared_ptr<Objects> objects,
                         std::shared_ptr<ControlPanel> controlPanel)
    : Scene(controlPanel), m_ModelRender(modelRender),
      m_VolumetricRender(volumetricRender), m_Objects(objects) {}

void SimpleScene::setup() {
    m_ModelRender->setup();
    // m_VolumetricRender->setup();
    m_Objects->setup();
}

void SimpleScene::update(float dt, glm::mat4 projection) {
    m_ModelRender->setDeltaTime(dt);
    m_ModelRender->setProjection(projection);
    m_ModelRender->setScale(m_ControlPanel->getScale());
    m_ModelRender->setRotation(m_ControlPanel->getRotation());
    m_ModelRender->setTranslation(m_ControlPanel->getTranslation());

    // m_VolumetricRender->setProjection(projection);
    // m_VolumetricRender->setScale(controlPanel->getScale());
    // m_VolumetricRender->setRotation(controlPanel->getRotation());
    // m_VolumetricRender->setTranslation(controlPanel->getTranslation());

    m_Objects->setTime(dt);
    m_Objects->setProjection(projection);
    m_Objects->setObjectType(m_ControlPanel->getObjectType());
}

void SimpleScene::render() {
    m_ModelRender->run();
    // m_VolumetricRender->run();
    m_Objects->run();
}