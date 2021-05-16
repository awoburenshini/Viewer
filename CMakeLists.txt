cmake_minimum_required(VERSION 3.15)

project(Viewer)

include_directories("./include")
link_directories("./lib/x64")

add_executable(Viewer src/glad.c src/main.cpp)

target_link_libraries(Viewer glfw3dll glew32 opengl32)