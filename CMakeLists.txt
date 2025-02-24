cmake_minimum_required(VERSION 3.12)

set(project_name "BMP-Converter")
set(CMAKE_CXX_STANDARD 14)

project(${project_name})

set(sources
        src/main.cpp
        src/HandleGuard/WinApiHandle.cpp
        src/BMP/BMP.cpp
        src/BMP/ioProcessBMP.cpp)


add_executable(${project_name} ${sources})

target_include_directories(${project_name} PUBLIC src)