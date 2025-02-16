cmake_minimum_required(VERSION 3.2)

set(project_name "BMP-Converter")
set(CMAKE_CXX_STANDARD 14)

project(${project_name})

set(sources src/main.cpp)

add_executable(${project_name} ${sources})