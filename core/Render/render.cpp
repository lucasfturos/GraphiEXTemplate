#include "render.hpp"

Render::Render(std::shared_ptr<MultiScenesOption> multiScenesOption)
    : Render(nullptr, multiScenesOption) {}

Render::Render(std::shared_ptr<ControlPanel> controlPanel,
               std::shared_ptr<MultiScenesOption> multiScenesOption)
    : m_Window(nullptr), m_Context(nullptr), m_Quit(false),
      m_PreviousSceneIndex(0), m_ControlPanel(controlPanel),
      m_MultiScenesOption(multiScenesOption) {}

Render::~Render() { destroyWindow(); }

void Render::setup() {
    setupWindow();
    initOpenGL();
    setupImGui();

    m_ControlPanel->setup();

    auto currentScene = m_MultiScenesOption->getCurrentScene();
    if (currentScene)
        currentScene->setup();
}

void Render::update(float time) {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);
    float aspectRatio =
        static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    glm::mat4 projMat =
        glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    auto currentScene = m_MultiScenesOption->getCurrentScene();
    std::size_t currentSceneIndex = m_MultiScenesOption->getCurrentSceneIndex();

    if (currentSceneIndex != m_PreviousSceneIndex) {
        if (currentScene) {
            currentScene->resetMatrix();
            currentScene->setup();
        }

        m_PreviousSceneIndex = currentSceneIndex;
    }

    if (currentScene)
        currentScene->update(time, projMat);
}

void Render::render() {
    auto currentScene = m_MultiScenesOption->getCurrentScene();
    if (currentScene)
        currentScene->render();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    m_ControlPanel->run();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Render::run() {
    setup();

    Uint32 lastTime = SDL_GetTicks();
    while (!m_Quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        m_FrameStart = SDL_GetTicks();
        handleEvents();

        clear();
        update(deltaTime);
        render();

        SDL_GL_SwapWindow(m_Window);
        m_FrameTime = SDL_GetTicks() - m_FrameStart;
        if (m_FrameDelay > m_FrameTime) {
            SDL_Delay(m_FrameDelay - m_FrameTime);
        }
    }
}