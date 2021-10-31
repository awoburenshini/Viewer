cmake_minimum_required(VERSION 3.15)

set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_GENERATOR "Visual Studio 15 2017")
message("generator is set to ${CMAKE_GENERATOR}")
project(Viewer)

# set our system with float
add_definitions(-DSINGLE_PRECISION)

include_directories("${CMAKE_SOURCE_DIR}/extern")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/extern/eigen3")
link_directories("${CMAKE_SOURCE_DIR}/lib/x64")

set(INC 
    include/common.h 
    include/Viewer.h 
    include/extensionViewer.h
    include/MeshIO.h
    )
set(SRCS 
    src/glad.c 
    src/Viewer.cpp
    src/MeshIO.cpp
    test/test0.cpp 
    # test/main.cpp
    )
add_executable(Viewer ${INC} ${SRCS})

target_link_libraries(Viewer glfw3dll glew32 opengl32)