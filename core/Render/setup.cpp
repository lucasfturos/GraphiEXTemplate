#include "render.hpp"

void Render::checkOpenGLErrors() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << std::hex << err << '\n';
    }
}

void Render::clear() {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    checkOpenGLErrors();
}

void Render::initOpenGL() {
    clear();
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Render::setupImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_Window, m_Context);
    ImGui_ImplOpenGL3_Init("#version 440");
    checkOpenGLErrors();
}

void Render::setupWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Error initializing SDL: " +
                                 std::string(SDL_GetError()));

    try {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);

        m_Window = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, m_ScreenWidth,
                                    m_ScreenHeight,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (!m_Window)
            throw std::runtime_error("Error creating window: " +
                                     std::string(SDL_GetError()));

        m_Context = SDL_GL_CreateContext(m_Window);
        if (!m_Context)
            throw std::runtime_error("Error creating context: " +
                                     std::string(SDL_GetError()));

        SDL_GL_MakeCurrent(m_Window, m_Context);

        if (SDL_GL_SetSwapInterval(1) < 0)
            throw std::runtime_error("Error setting VSync: " +
                                     std::string(SDL_GetError()));

        GLenum err = glewInit();
        if (err != GLEW_OK)
            throw std::runtime_error(
                "Error initializing GLEW: " +
                std::string((const char *)glewGetErrorString(err)));

    } catch (const std::runtime_error &) {
        destroyWindow();
        throw;
    }
}

void Render::destroyWindow() {
    if (m_Window) {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }
    if (m_Context) {
        SDL_GL_DeleteContext(m_Context);
        m_Context = nullptr;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_Quit();
}