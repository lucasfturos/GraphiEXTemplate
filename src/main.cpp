#include "Scenes/simple_scene.hpp"
#include "Scenes/volumetric_scene.hpp"

int main(void) {
    try {
        // Examples
        auto modelRender = std::make_shared<ModelRender>(
            "assets/model/Nightshade/Breakdance Freeze Var 2.dae");
        auto volumetricRender =
            std::make_shared<VolumetricRender>("assets/model/spot.obj");
        auto objects = std::make_shared<Objects>();

        if (!modelRender || !volumetricRender || !objects) {
            throw std::runtime_error("Failed to initialize render objects.");
            return -1;
        }

        // Scenes render
        auto controlPanel = std::make_shared<ControlPanel>();
        auto simpleScene =
            std::make_shared<SimpleScene>(controlPanel, modelRender, objects);
        auto volumetricScene =
            std::make_shared<VolumetricScene>(controlPanel, volumetricRender);

        if (!controlPanel || !simpleScene || !volumetricScene) {
            throw std::runtime_error("Failed to initialize scenes objects.");
            return -1;
        }

        // Render
        // auto render = std::make_shared<Render>(simpleScene, controlPanel);
        auto render = std::make_shared<Render>(volumetricScene, controlPanel);
        render->run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}