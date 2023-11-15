cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0048 NEW)
project(ShapeShifter)
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glad)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17")

add_library(ShapeShifter_lib STATIC 
    src/glfw_application.cpp
    src/main_loop.cpp
    src/renderer.cpp
    src/input.cpp
    src/shape.cpp
    src/game_logic.cpp
    third_party/glad/GL/src/gl.c
    )

include_directories(ShapeShifter_lib PUBLIC
    include
    third_party/glad/GL/include
    third_party/glfw/include)

add_executable(ShapeShifter 
    main.cpp
)

target_link_libraries(ShapeShifter ShapeShifter_lib glfw ${GLFW_LIBRARIES})