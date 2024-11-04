#include "control_panel.hpp"

void ControlPanel::gizmoWindow() {
    if (showGizmoWindow) {
        setupGizmoWindow();
        renderGizmoWindow();
        renderGizmoControls();
    }
}

void ControlPanel::setupGizmoWindow() {
    ImGui::Begin("Gizmo", &showGizmoWindow,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - gizmoWidth, 0));
    ImGui::SetWindowSize(ImVec2(gizmoWidth, gizmoHeight));

    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    framebuffer->rescaleFrameBuffer(windowSize.x, windowSize.y);

    framebuffer->bind();
    glViewport(0, 0, windowSize.x, windowSize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Mesh<MeshTypes<glm::vec3, GLuint>>::UniformsMap
ControlPanel::createUniforms(const ImVec2 &windowSize) {
    return {
        {"uMVP", [this, windowSize](std::shared_ptr<Shader> shader) {
             glm::mat4 model = glm::mat4(1.0f);

             glm::vec3 adjustedTranslation = translation * (1.0f / scaleFactor);
             model = glm::translate(model, adjustedTranslation);

             model =
                 glm::rotate(model, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
             model =
                 glm::rotate(model, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));

             model = glm::scale(model, scale);

             glm::mat4 view =
                 glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
             glm::mat4 projection =
                 glm::perspective(glm::radians(45.0f),
                                  windowSize.x / windowSize.y, 0.1f, 100.0f);

             glm::mat4 mvp = projection * view * model;
             shader->setUniformMat4f("uMVP", mvp);
         }}};
}

void ControlPanel::renderGizmoWindow() {
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowMax =
        ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y);

    handleMouseInteraction(windowPos, windowMax);

    Mesh<MeshTypes<glm::vec3, GLuint>>::UniformsMap uniforms =
        createUniforms(windowSize);
    cubeMesh->setUniforms(uniforms);
    cubeMesh->draw();

    framebuffer->unbind();
    ImGui::Image(reinterpret_cast<void *>(framebuffer->getTexture()),
                 windowSize);
    ImGui::End();
}

void ControlPanel::renderGizmoControls() {
    ImGui::Begin("Gizmo Controls", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::SetWindowPos(ImVec2(0, bottomControlPanelHeight + 10));
    ImGui::SetWindowSize(ImVec2(gizmoControlWidth, gizmoControlHeight));

    ImGui::PushItemWidth(gizmoControlWidth - 15);

    ImGui::Text("Translation");
    ImGui::SliderFloat3("##Translation", glm::value_ptr(translation), -10.0f,
                        10.0f, "%.2f");

    ImGui::Text("Scale");
    ImGui::SliderFloat3("##Scale", glm::value_ptr(scale), 0.1f, 10.0f, "%.2f");

    ImGui::Spacing();

    ImGui::Text("Mode");
    if (ImGui::Button("Rotate")) {
        currentMode = TransformMode::Rotate;
    }
    ImGui::SameLine();
    if (ImGui::Button("Scale")) {
        currentMode = TransformMode::Scale;
    }
    ImGui::SameLine();
    if (ImGui::Button("Translate")) {
        currentMode = TransformMode::Translate;
    }

    ImGui::Text("Scale Factor");
    ImGui::SliderFloat("##ScaleFactor", &scaleFactor, 0.01f, 100.0f, "%.2f");

    ImGui::Spacing();
    if (ImGui::Button("Reset")) {
        resetTransformations();
    }
    ImGui::PopItemWidth();

    ImGui::End();
}

void ControlPanel::mainWindow() {
    ImGui::Begin("Control Panel", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2(controlPanelWidth, controlPanelHeight));

    ImVec2 controlPanelPos = ImGui::GetWindowPos();
    ImVec2 controlPanelSize = ImGui::GetWindowSize();
    bottomControlPanelHeight = controlPanelPos.y + controlPanelSize.y;

    ImGui::PushItemWidth(200);

    renderSceneSelector();

    renderObjectTypeSelector();

    if (ImGui::Button("Gizmo Window")) {
        showGizmoWindow = !showGizmoWindow;
    }

    ImGui::PopItemWidth();

    ImGui::End();
}

void ControlPanel::renderObjectTypeSelector() {
    std::vector<const char *> objectTypeNames = {"None", "Sphere", "Cylinder",
                                                 "Plane", "Cube"};
    int currentType = static_cast<int>(objectType);
    if (ImGui::Combo("##comboobject", &currentType, objectTypeNames.data(),
                     objectTypeNames.size())) {
        objectType = static_cast<ObjectType>(currentType);
    }

    ImGui::SameLine();
    if (ImGui::Button("Default")) {
        objectType = ObjectType::None;
    }
}

void ControlPanel::renderSceneSelector() {
    std::vector<std::string> sceneNames = m_MultiScenesOption->getSceneNames();
    std::vector<const char *> sceneNamesCStr;
    for (const auto &name : sceneNames) {
        sceneNamesCStr.push_back(name.c_str());
    }

    int selectedScene = m_MultiScenesOption->getCurrentSceneIndex();
    if (ImGui::Combo("  Scenes", &selectedScene, sceneNamesCStr.data(),
                     sceneNamesCStr.size())) {
        m_MultiScenesOption->setCurrentSceneIndex(selectedScene);
    }
}
