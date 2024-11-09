# -------------------------------------------------------------------

set(TEST_SOURCES
    # Common
    core/Common/test_color.cpp
    core/Common/test_perlin.cpp
    core/Common/test_util.cpp
    # Objects3D
    core/Objects3D/test_cylinder.cpp
    core/Objects3D/test_plane.cpp
    core/Objects3D/test_sphere.cpp
    # main
    main.cpp
)

find_package(glm REQUIRED)
find_package(GLEW REQUIRED)

add_executable(Tests
    ${TEST_SOURCES}
)

target_include_directories(Tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

if(TARGET Core_lib)
    target_link_libraries(Tests PUBLIC Core_lib)
endif()

target_link_libraries(Tests PUBLIC
    glm::glm
    GLEW::GLEW
)

set(CTEST_PROGRESS_OUTPUT ON)
set(CTEST_OUTPUT_ON_FAILURE ON)

add_test(NAME Tests COMMAND Tests)

# -------------------------------------------------------------------