cmake_minimum_required(VERSION 3.5)

project(MyProg.cpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDART 17)

add_executable(MyProg MyProg.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
