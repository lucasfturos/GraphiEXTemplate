# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(SRC_HEADER
    Model/model_render.hpp
    Objects/objects.hpp
    SimpleScene/simple_scene.hpp
    VolumetricRender/volumetric_render.hpp
)

set(SRC_SOURCE
    Model/model_render.cpp
    Objects/objects.cpp
    SimpleScene/simple_scene.cpp
    VolumetricRender/volumetric_render.cpp
    main.cpp
)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} 
    ${SRC_HEADER} 
    ${SRC_SOURCE}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Core_lib
    External_lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    # SDL2
    ${SDL2_LIBRARIES}
    # OpenGL
    glm::glm
    GLEW::GLEW
    ${CMAKE_DL_LIBS}
    ${OPENGL_LIBRARIES}
)

# -------------------------------------------------------------------