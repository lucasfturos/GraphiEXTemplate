# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(CORE_HEADERS
    # ControlPanel
    ControlPanel/control_panel.hpp
    # GLObjects
    GLObjects/Mesh/mesh.hpp
    GLObjects/Texture/texture.hpp
    GLObjects/VBLayout/vertex_buffer_layout.hpp
    # Render
    Render/render.hpp
)

set(CORE_SOURCES
    # ControlPanel
    ControlPanel/control_panel.cpp
    ControlPanel/gui.cpp
    ControlPanel/window.cpp
    # GLObjects
    GLObjects/Mesh/mesh.cpp
    GLObjects/Texture/texture.cpp
    GLObjects/VBLayout/vertex_buffer_layout.cpp
    # Render
    Render/event.cpp
    Render/render.cpp
    Render/setup.cpp    
)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

add_library(Core_lib STATIC 
    ${CORE_HEADERS}
    ${CORE_SOURCES}
)

target_include_directories(Core_lib PRIVATE
    ${ASSIMP_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

target_include_directories(Core_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(Core_lib PUBLIC
    External_lib
)

target_link_libraries(Core_lib PRIVATE
    # Assimp
    ${ASSIMP_LIBRARIES}
    # OpenGL
    glm::glm
    GLEW::GLEW
    ${CMAKE_DL_LIBS}
    ${OPENGL_LIBRARIES}
)
