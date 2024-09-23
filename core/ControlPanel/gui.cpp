#include "control_panel.hpp"
#include "imgui.h"

void ControlPanel::initFont() {
    ImGui::GetIO().Fonts->Clear();
    ImGui::GetIO().Fonts->AddFontFromFileTTF("./assets/font/Roboto-Regular.ttf",
                                             18.0f);
}

void ControlPanel::styleWidget() {
    ImGuiStyle &style = ImGui::GetStyle();
    // Window
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;

    style.Colors[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 0.9f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.3f, 0.3f, 0.3f, 0.3f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.3f, 0.3f, 0.3f, 0.3f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.9f, 0.8f, 0.8f, 0.4f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.9f, 0.6f, 0.6f, 0.4f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.8f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.1f, 0.1f, 0.8f);
    // Botões
    style.Colors[ImGuiCol_Button] = ImVec4(0.16f, 0.16f, 0.16f, 0.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.16f, 0.16f, 0.5f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.31f, 0.28f, 0.28f, 1.0f);
    // Slider
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive] =
        ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    // RadioButton
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    // Header
    style.Colors[ImGuiCol_Header] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
}

void ControlPanel::mainWindow() {
    ImGui::Begin("Control Panel", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));

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

    if (ImGui::Button("Gizmo Window")) {
        showGizmoWindow = !showGizmoWindow;
    }

    ImGui::End();
}

void ControlPanel::gizmoWindow() {
    if (showGizmoWindow) {
        ImGui::Begin("Gizmo", &showGizmoWindow,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowPos(
            ImVec2(ImGui::GetIO().DisplaySize.x - gizmoWidth, 0));
        ImGui::SetWindowSize(ImVec2(gizmoWidth, gizmoHeight));

        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        framebuffer->rescaleFrameBuffer(windowSize.x, windowSize.y);

        framebuffer->bind();

        glViewport(0, 0, windowSize.x, windowSize.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowMax =
            ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y);

        if (ImGui::IsMouseHoveringRect(windowPos, windowMax)) {
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                ImVec2 mouseDelta =
                    ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
                rotation.x += mouseDelta.x * 0.03f;
                rotation.y += mouseDelta.y * 0.03f;
                ImGui::ResetMouseDragDelta();
            }
        }

        Mesh<glm::vec3, GLuint>::UniformsMap uniforms = {
            {"uMVP", [this, windowSize](std::shared_ptr<Shader> shader) {
                 glm::mat4 model = glm::mat4(1.0f);
                 model = glm::rotate(model, rotation.x,
                                     glm::vec3(0.0f, 1.0f, 0.0f));
                 model = glm::rotate(model, rotation.y,
                                     glm::vec3(1.0f, 0.0f, 0.0f));

                 glm::mat4 view = glm::translate(glm::mat4(1.0f),
                                                 glm::vec3(0.0f, 0.0f, -5.0f));

                 glm::mat4 projection = glm::perspective(
                     glm::radians(45.0f), windowSize.x / windowSize.y, 0.1f,
                     100.0f);

                 glm::mat4 mvp = projection * view * model;
                 shader->setUniformMat4f("uMVP", mvp);
             }}};

        cubeMesh->setUniforms(uniforms);
        cubeMesh->draw();
        framebuffer->unbind();

        ImGui::Image(reinterpret_cast<void *>(framebuffer->getTexture()),
                     windowSize);
        ImGui::End();
    }
}
