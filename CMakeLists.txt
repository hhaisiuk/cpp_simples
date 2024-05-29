cmake_minimum_required(VERSION 3.16..3.5)

project(simples VERSION 0.1.0 LANGUAGES C CXX)

option(BUILD_TESTING OFF)

enable_testing()

set(CMAKE_CXX_STANDART 17)

add_executable(${PROJECT_NAME} simples.cpp)

add_subdirectory(extern)
add_subdirectory(test)