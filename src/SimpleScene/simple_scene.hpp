#pragma once

#include "ControlPanel/control_panel.hpp"
#include "Model/model_render.hpp"
#include "Objects/objects.hpp"
#include "Render/render.hpp"
#include "VolumetricRender/volumetric_render.hpp"

class SimpleScene : public Scene {
  private:
    std::shared_ptr<ModelRender> m_ModelRender;
    std::shared_ptr<VolumetricRender> m_VolumetricRender;
    std::shared_ptr<Objects> m_Objects;

  public:
    SimpleScene(std::shared_ptr<ModelRender>, std::shared_ptr<VolumetricRender>,
                std::shared_ptr<Objects>, std::shared_ptr<ControlPanel>);

    void setup() override;
    void update(float, glm::mat4) override;
    void render() override;
};
