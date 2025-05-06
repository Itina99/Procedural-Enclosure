cmake_minimum_required(VERSION 3.29)
project(Procedural_Enclosure)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/glfw-3.4)
include_directories( SYSTEM lib/glad/include
        include)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
        src/glad.c
        src/utils.cpp
        include/utils.h
        src/camera.cpp
        include/camera.h
        src/mesh.cpp
        include/mesh.h
        src/shader.cpp
        include/shader.h
        include/NoiseGenerator.h
        src/NoiseGenerator.cpp
        include/PoissonGenerator.h
        src/PoissonGenerator.cpp
        src/stb_image.cpp
        src/lindenmayer.cpp
        include/lindenmayer.h
        include/abstract_builder.h
        src/branch_builder.cpp
        include/branch_builder.h
        src/leaf_builder.cpp
        include/leaf_builder.h
        src/interpreter.cpp
        include/interpreter.h
        src/tree.cpp
        include/tree.h)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} glm::glm)


