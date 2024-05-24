# set minimum cmake version
cmake_minimum_required(VERSION 3.29)

# project name and language
project(simples LANGUAGES CXX)

# require C++17
set(CMAKE_CXX_STANDARD 17)

# main code
add_executable(${PROJECT_NAME} simples.cpp)

# enable testing functionality
enable_testing()

# add subdirectory
add_subdirectory(test)
