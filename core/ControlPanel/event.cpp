#include "control_panel.hpp"

void ControlPanel::handleMouseInteraction(const ImVec2 &windowPos,
                                          const ImVec2 &windowMax) {
    if (ImGui::IsMouseHoveringRect(windowPos, windowMax) &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ImVec2 mouseDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

        switch (currentMode) {
        case TransformMode::Rotate:
            rotation.x += mouseDelta.x * 0.03f;
            rotation.y += mouseDelta.y * 0.03f;
            break;
        case TransformMode::Translate:
            translation.x += mouseDelta.x * 0.01f * scaleFactor;
            translation.y += mouseDelta.y * 0.01f * scaleFactor;
            break;
        case TransformMode::Scale:
            scale += glm::vec3(mouseDelta.y * 0.01f * scaleFactor);
            scale = glm::max(scale, glm::vec3(0.05f));
            break;
        }

        ImGui::ResetMouseDragDelta();
    }
}

void ControlPanel::resetTransformations() {
    translation = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
}
