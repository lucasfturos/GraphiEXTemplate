#include "render.hpp"

Render::Render()
    : window(nullptr), context(nullptr),
      objects(std::make_shared<Objects>(viewDefaultMat, projDefaultMat)),
      modelRender(std::make_shared<ModelRender>(
          "assets/model/spot.obj", viewDefaultMat, projDefaultMat)),
      controlPanel(std::make_shared<ControlPanel>()), quit(false) {
    setupWindow();
    initOpenGL();
    setupImGui();
}

Render::~Render() { destroyWindow(); }

void Render::setup() {
    controlPanel->setup();
    modelRender->setup();
    objects->setup();
}

void Render::draw() {
    modelRender->setRotation(controlPanel->getRotation());

    modelRender->run();
    objects->setObjectType(controlPanel->getObjectType());
    objects->run();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    controlPanel->run();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Render::run() {
    static float t = 0;

    setup();
    Uint32 lastTime = SDL_GetTicks();

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        t += deltaTime;

        objects->setTime(t);

        frameStart = SDL_GetTicks();
        handleEvents();

        clear();
        draw();

        SDL_GL_SwapWindow(window);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}