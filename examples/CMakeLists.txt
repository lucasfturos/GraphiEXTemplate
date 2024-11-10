# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(EXAMPLES_HEADER
    Model/model_render.hpp
    Objects/objects.hpp
    Volume/volume_obj.hpp
)

set(EXAMPLES_SOURCE
    Model/model_render.cpp
    Objects/objects.cpp
    Volume/volume_obj.cpp
)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

add_library(Examples_lib STATIC 
    ${EXAMPLES_HEADER} 
    ${EXAMPLES_SOURCE}
)

target_include_directories(Examples_lib PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(Examples_lib PUBLIC
    Core_lib
    External_lib
)

target_link_libraries(Examples_lib PRIVATE
    # SDL2
    ${SDL2_LIBRARIES}
    # OpenGL
    glm::glm
    GLEW::GLEW
    ${CMAKE_DL_LIBS}
    ${OPENGL_LIBRARIES}
)

# -------------------------------------------------------------------