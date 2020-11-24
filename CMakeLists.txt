cmake_minimum_required(VERSION 3.18)
project(wg_graph)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(libs/SFML)
include_directories(src)

add_executable(wg_graph
        src/main.cpp
        src/Window/Window.cpp
        src/Window/Window.h
        src/Game/Game.cpp
        src/Game/Game.h)

#find_package(SFML COMPONENTS graphics audio REQUIRED)

target_link_libraries(wg_graph sfml-graphics)
