#include "control_panel.hpp"
#include <vector>

ControlPanel::ControlPanel() : objectType(ObjectType::None) {}

void ControlPanel::setup() {
    styleWidget();
    initFont();
}

void ControlPanel::run() {
    ImGui::Begin("Control Panel");
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2(width, height));

    ImGui::PushItemWidth(200);
    std::vector<const char *> objectTypeNames = {"None", "Sphere", "Cylinder",
                                                 "Plane", "Cube"};
    int currentType = static_cast<int>(objectType);
    if (ImGui::Combo(" ", &currentType, objectTypeNames.data(),
                     objectTypeNames.size())) {
        objectType = static_cast<ObjectType>(currentType);
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Default")) {
        objectType = ObjectType::None;
    }

    ImGui::End();
}
