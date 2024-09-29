#include "render.hpp"

Render::Render(std::shared_ptr<Scene> scene,
               std::shared_ptr<ControlPanel> controlPanel)
    : window(nullptr), context(nullptr), quit(false), currentScene(scene),
      controlPanel(controlPanel) {
    setupWindow();
    initOpenGL();
    setupImGui();
}

Render::~Render() { destroyWindow(); }

void Render::setup() {
    controlPanel->setup();
    currentScene->setup();
}

void Render::update(float time) {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    float aspectRatio =
        static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    glm::mat4 projMat =
        glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    currentScene->update(time, projMat);
}

void Render::render() {
    currentScene->render();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    controlPanel->run();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Render::run() {
    static float t = 0.0;

    setup();

    Uint32 lastTime = SDL_GetTicks();
    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        t += deltaTime;

        frameStart = SDL_GetTicks();
        handleEvents();

        clear();
        update(t);
        render();

        SDL_GL_SwapWindow(window);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}