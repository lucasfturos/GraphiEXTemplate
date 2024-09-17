# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(CORE_HEADERS
    # ControlPanel
    ControlPanel/control_panel.hpp
    # GLObjects
    GLObjects/read_object_file.hpp
    GLObjects/vertex_buffer_layout.hpp
)

set(CORE_SOURCES
    # ControlPanel
    ControlPanel/control_panel.cpp
    ControlPanel/gui.cpp
    # GLObjects
    GLObjects/read_object_file.cpp
    GLObjects/vertex_buffer_layout.cpp
)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)

add_library(Core_lib STATIC 
    ${CORE_HEADERS}
    ${CORE_SOURCES}
)

target_include_directories(Core_lib PUBLIC
    ${OPENGL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(Core_lib PUBLIC
    External_lib
)

target_link_libraries(Core_lib PRIVATE
    # OpenGL
    glm::glm
    GLEW::GLEW
    ${CMAKE_DL_LIBS}
    ${OPENGL_LIBRARIES}
)
