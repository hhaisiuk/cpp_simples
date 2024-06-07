cmake_minimum_required(VERSION 3.16..3.5)
project(Tests)

if(BUILD_TESTING)
    add_executable(${PROJECT_NAME} test.cpp)

    target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)
    target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gmock_main)

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME})
endif()


