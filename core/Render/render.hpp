#pragma once

#include "ControlPanel/control_panel.hpp"
#include "scene.hpp"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

class Render {
  protected:
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const int fps = 60;
    const Uint32 frameDelay = 1000.0 / fps;
    const std::string title = "Render";

  private:
    SDL_Window *window;
    SDL_GLContext context;
    bool quit;
    std::shared_ptr<Scene> currentScene;
    std::shared_ptr<ControlPanel> controlPanel;

  private:
    Uint32 frameStart;
    Uint32 frameTime;

    void update(float);

    // Setup
    void clear();
    void initOpenGL();
    void setupImGui();
    void setupWindow();
    void destroyWindow();

    // Event
    void handleEvents();

  public:
    Render(std::shared_ptr<Scene> scene,
           std::shared_ptr<ControlPanel> controlPanel);
    ~Render();

    void setScene(std::shared_ptr<Scene> scene) { currentScene = scene; }

    void run();
};