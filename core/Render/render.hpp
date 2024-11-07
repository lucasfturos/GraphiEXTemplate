#pragma once

#include "ControlPanel/control_panel.hpp"
#include "Scenes/multi_scenes_option.hpp"
#include "Scenes/scene.hpp"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <memory>

class Render {
  protected:
    const int m_ScreenWidth = 1280;
    const int m_ScreenHeight = 720;
    const int m_FPS = 60;
    const Uint32 m_FrameDelay = 1000.0 / m_FPS;
    const std::string m_Title = "Render";

  private:
    SDL_Window *m_Window;
    SDL_GLContext m_Context;

    bool m_Quit;
    std::size_t m_PreviousSceneIndex;

    std::shared_ptr<ControlPanel> m_ControlPanel;
    std::shared_ptr<MultiScenesOption> m_MultiScenesOption;

  private:
    Uint32 m_FrameStart;
    Uint32 m_FrameTime;

    void update(float);
    void render();

    // Setup
    void clear();
    void setup();
    void initOpenGL();
    void setupImGui();
    void setupWindow();
    void destroyWindow();
    void checkOpenGLErrors();

    // Event
    void handleEvents();

  public:
    Render(std::shared_ptr<ControlPanel>, std::shared_ptr<MultiScenesOption>);
    ~Render();

    void run();
};