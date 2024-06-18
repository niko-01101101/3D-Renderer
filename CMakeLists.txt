project(MyGL)
cmake_minimum_required(VERSION 3.10)
set(SOURCES src/window.cpp src/main.cpp src/renderer.cpp src/model.cpp src/window.h src/renderer.h src/model.h src/geometry.h src/render_types.h)
add_executable(renderer ${SOURCES})
target_include_directories(renderer PUBLIC
                           "$src"
                           )