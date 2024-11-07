# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(SRC_SOURCE
    main.cpp
)

add_executable(${PROJECT_NAME} 
    ${SRC_SOURCE}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/examples
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Examples_lib
)

# -------------------------------------------------------------------