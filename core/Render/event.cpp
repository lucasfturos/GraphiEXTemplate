#include "render.hpp"

void Render::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            m_Quit = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                m_Quit = true;
                break;
            }
            break;
        default:
            break;
        }
        
        auto currentScene = m_MultiScenesOption->getCurrentScene();
        if (currentScene)
            currentScene->handleEvent(event);
    }
}