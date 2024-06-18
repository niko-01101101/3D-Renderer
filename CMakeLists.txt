project(MyGL)
cmake_minimum_required(VERSION 3.10)
set(SOURCES src/window.cpp src/main.cpp src/renderer.cpp src/window.h src/renderer.h)
add_executable(renderer ${SOURCES})
target_include_directories(renderer PUBLIC
                           "$src"
                           )