# -------------------------------------------------------------------

set(TEST_SOURCES
    # Common
    core/Common/test_color.cpp
    core/Common/test_perlin.cpp
    core/Common/test_util.cpp
    # GLObjects
    core/GLObjects/test_interpolation.cpp
    # Objects3D
    core/Objects3D/test_cylinder.cpp
    core/Objects3D/test_sphere.cpp
    # root
    util.cpp
    main.cpp
)

find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

add_executable(Tests
    ${TEST_SOURCES}
)

target_include_directories(Tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${ASSIMP_INCLUDE_DIRS}
)

if(TARGET Core_lib)
    target_link_libraries(Tests PUBLIC Core_lib)
endif()

target_link_libraries(Tests PUBLIC
    ${ASSIMP_LIBRARIES}
    glm::glm
    GLEW::GLEW
)

set(CTEST_PROGRESS_OUTPUT ON)
set(CTEST_OUTPUT_ON_FAILURE ON)

add_test(NAME Tests COMMAND Tests)

# -------------------------------------------------------------------