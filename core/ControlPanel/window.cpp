#include "control_panel.hpp"

void ControlPanel::gizmoWindow() {
    if (m_ShowGizmoWindow) {
        setupGizmoWindow();
        renderGizmoWindow();
        renderGizmoControls();
    }
}

void ControlPanel::setupGizmoWindow() {
    ImGui::Begin("Gizmo", &m_ShowGizmoWindow,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - m_GizmoWidth, 0));
    ImGui::SetWindowSize(ImVec2(m_GizmoWidth, m_GizmoHeight));

    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    m_Framebuffer->rescaleFrameBuffer(windowSize.x, windowSize.y);

    m_Framebuffer->bind();
    glViewport(0, 0, windowSize.x, windowSize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Mesh<MeshTypes<glm::vec3, GLuint>>::UniformsMap
ControlPanel::createUniforms(const ImVec2 &windowSize) {
    return {
        {"uMVP", [this, windowSize](std::shared_ptr<Shader> shader) {
             glm::mat4 model = glm::mat4(1.0f);

             glm::vec3 adjustedTranslation =
                 m_Translation * (1.0f / m_ScaleFactor);
             model = glm::translate(model, adjustedTranslation);

             model =
                 glm::rotate(model, m_Rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
             model =
                 glm::rotate(model, m_Rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));

             model = glm::scale(model, m_Scale);

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
    m_CubeMesh->setUniforms(uniforms);
    m_CubeMesh->draw();

    m_Framebuffer->unbind();
    ImGui::Image(reinterpret_cast<void *>(m_Framebuffer->getTexture()),
                 windowSize);
    ImGui::End();
}

void ControlPanel::renderGizmoControls() {
    ImGui::Begin("Gizmo Controls", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::SetWindowPos(ImVec2(0, m_BottomControlPanelHeight + 10));
    ImGui::SetWindowSize(ImVec2(m_GizmoControlWidth, m_GizmoControlHeight));

    ImGui::PushItemWidth(m_GizmoControlWidth - 15);

    ImGui::Text("Translation");
    ImGui::SliderFloat3("##Translation", glm::value_ptr(m_Translation), -10.0f,
                        10.0f, "%.2f");

    ImGui::Text("Scale");
    ImGui::SliderFloat3("##Scale", glm::value_ptr(m_Scale), 0.1f, 10.0f,
                        "%.2f");

    ImGui::Spacing();

    ImGui::Text("Mode");
    if (ImGui::Button("Rotate")) {
        m_CurrentMode = TransformMode::Rotate;
    }
    ImGui::SameLine();
    if (ImGui::Button("Scale")) {
        m_CurrentMode = TransformMode::Scale;
    }
    ImGui::SameLine();
    if (ImGui::Button("Translate")) {
        m_CurrentMode = TransformMode::Translate;
    }

    ImGui::Text("Scale Factor");
    ImGui::SliderFloat("##ScaleFactor", &m_ScaleFactor, 0.01f, 100.0f, "%.2f");

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
    ImGui::SetWindowSize(ImVec2(m_ControlPanelWidth, m_ControlPanelHeight));

    ImVec2 controlPanelPos = ImGui::GetWindowPos();
    ImVec2 controlPanelSize = ImGui::GetWindowSize();
    m_BottomControlPanelHeight = controlPanelPos.y + controlPanelSize.y;

    ImGui::PushItemWidth(200);

    renderSceneSelector();

    renderObjectTypeSelector();

    if (ImGui::Button("Gizmo Window")) {
        m_ShowGizmoWindow = !m_ShowGizmoWindow;
    }

    ImGui::PopItemWidth();

    ImGui::End();
}

void ControlPanel::renderObjectTypeSelector() {
    std::vector<const char *> objectTypeNames = {"None", "Sphere", "Cylinder",
                                                 "Plane", "Cube"};
    int currentType = static_cast<int>(m_ObjectType);
    if (ImGui::Combo("##comboobject", &currentType, objectTypeNames.data(),
                     objectTypeNames.size())) {
        m_ObjectType = static_cast<ObjectType>(currentType);
    }

    ImGui::SameLine();
    if (ImGui::Button("Default")) {
        m_ObjectType = ObjectType::None;
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
