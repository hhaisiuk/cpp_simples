project(PointerTests)
if(BUILD_TESTING)
    add_executable(${PROJECT_NAME} PointerTests.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME})
endif()


