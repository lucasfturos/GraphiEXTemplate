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

void Render::run() {
    static float t = 0.0;

    controlPanel->setup();
    currentScene->setup();

    Uint32 lastTime = SDL_GetTicks();
    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        t += deltaTime;

        frameStart = SDL_GetTicks();
        handleEvents();

        clear();

        currentScene->update(t);
        currentScene->render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        controlPanel->run();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}