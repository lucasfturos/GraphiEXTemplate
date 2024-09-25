#include "Model/model_render.hpp"
#include "Objects/objects.hpp"
#include "Render/render.hpp"
#include "SimpleScene/simple_scene.hpp"

int main(void) {
    try {
        auto modelRender = std::make_shared<ModelRender>(
            "assets/model/Nightshade/Breakdance_1990.dae");
        auto objects = std::make_shared<Objects>();

        auto controlPanel = std::make_shared<ControlPanel>();
        auto simpleScene =
            std::make_shared<SimpleScene>(modelRender, objects, controlPanel);

        auto render = std::make_shared<Render>(simpleScene, controlPanel);
        render->run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}