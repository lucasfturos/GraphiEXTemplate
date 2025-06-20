#include "ScenesRender/simple_scene.hpp"
#include "ScenesRender/softbody_scene.hpp"
#include "ScenesRender/volumetric_scene.hpp"

int main(void) {
    try {
        // Examples
        auto modelRender = std::make_shared<ModelRender>(
            "assets/model/Nightshade/Breakdance Freeze Var 2.dae");
        auto volumeObject =
            std::make_shared<VolumeObject>("assets/model/spot.obj");
        auto objects = std::make_shared<Objects>();

        if (!modelRender || !volumeObject || !objects) {
            throw std::runtime_error("Failed to initialize render objects.");
            return -1;
        }

        // Scenes render
        auto multiScenesOption = std::make_shared<MultiScenesOption>();
        auto controlPanel = std::make_shared<ControlPanel>(multiScenesOption);

        auto simpleScene =
            std::make_shared<SimpleScene>(controlPanel, modelRender, objects);
        auto softBodyScene = std::make_shared<SoftBodyScene>(controlPanel);
        auto volumetricScene =
            std::make_shared<VolumetricScene>(controlPanel, volumeObject);

        if (!controlPanel || !simpleScene || !softBodyScene ||
            !volumetricScene) {
            throw std::runtime_error("Failed to initialize scenes objects.");
            return -1;
        }

        // Add Scenes
        multiScenesOption->addScene(softBodyScene);
        multiScenesOption->addScene(simpleScene);
        multiScenesOption->addScene(volumetricScene);
        multiScenesOption->setCurrentSceneIndex(2);

        // Render
        auto render = std::make_shared<Render>(controlPanel, multiScenesOption);
        render->run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}