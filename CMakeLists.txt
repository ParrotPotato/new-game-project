cmake_minimum_required(VERSION 3.0)

# setting up debugging options

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(
    newgameproject
    src/main.cpp
    src/core.cpp
    src/utils.cpp
    src/shader.cpp
)

target_include_directories(newgameproject   PUBLIC  ${SDL2_INCLUDE_DIRS})
target_include_directories(newgameproject   PUBLIC  ${OPENGL_INCLUDE_DIRS})
target_include_directories(newgameproject   PUBLIC  ${GLEW_INCLUDE_DIRS})

target_link_libraries(newgameproject ${SDL2_LIBRARIES})
target_link_libraries(newgameproject ${OPENGL_LIBRARIES})
target_link_libraries(newgameproject ${GLEW_LIBRARIES})
