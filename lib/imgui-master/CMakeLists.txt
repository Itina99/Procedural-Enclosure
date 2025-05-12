cmake_minimum_required(VERSION 3.0)

file(GLOB IMGUI_SOURCES
        *.cpp
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
)

file(GLOB IMGUI_HEADERS
        *.h
        backends/imgui_impl_glfw.h
        backends/imgui_impl_opengl3.h
)

add_library(imgui ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} backends)
