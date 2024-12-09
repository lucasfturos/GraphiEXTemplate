#include "control_panel.hpp"

void ControlPanel::handleMouseInteraction(const ImVec2 &windowPos,
                                          const ImVec2 &windowMax) {
    if (ImGui::IsMouseHoveringRect(windowPos, windowMax) &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ImVec2 mouseDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

        switch (m_CurrentMode) {
        case TransformMode::Rotate:
            m_Rotation.x += mouseDelta.x * 0.03f;
            m_Rotation.y += mouseDelta.y * 0.03f;
            break;
        case TransformMode::Translate:
            m_Translation.x += mouseDelta.x * 0.01f * m_ScaleFactor;
            m_Translation.y += mouseDelta.y * 0.01f * m_ScaleFactor;
            break;
        case TransformMode::Scale:
            m_Scale += glm::vec3(mouseDelta.y * 0.01f * m_ScaleFactor);
            m_Scale = glm::max(m_Scale, glm::vec3(0.05f));
            break;
        }

        ImGui::ResetMouseDragDelta();
    }
}

void ControlPanel::resetTransformations() {
    m_Translation = glm::vec3(0.0f);
    m_Rotation = glm::vec3(0.0f);
    m_Scale = glm::vec3(1.0f);
}
