# set minimum cmake version
cmake_minimum_required(VERSION 3.5)

# create the testing file and list of tests
set(test_to_run
    pointer_construction_test.cpp
    pointer_invalid_construction_test.cpp

)
create_test_sourcelist(
    Tests PointerTestRunner.cpp ${test_to_run}
)

# add executable
add_executable(PointerTestRunner ${Tests})

# add tests 
foreach(test ${test_to_run})
    get_filename_component(test_name ${test} NAME_WE)
    add_test(
        NAME ${test_name}
        COMMAND PointerTestRunner ${test_name}
        LABELS PointerTest
    )
endforeach()













# block(PROPAGATE test_to_run)
# endbloc()
# file(GLOB files "*test.cpp")
# foreach(file ${files})
    
#     get_filename_component(test_file_name ${file} NAME)
#     message(STATUS "Test file name: ${test_file_name}")
#     list(PARENT_SCOPE ${test_to_run}  ${test_file_name})
#     message(STATUS "Test list: ${test_to_run}")
    
# endforeach()

# create_test_sourcelist(Tests PointerTestRunner.cpp ${test_to_run})

# # build tests
# function(build_tests)
#     file(GLOB files "*test.cpp")
  
#     foreach(file ${files})
#         get_filename_component(exe_name ${file} NAME)
#         add_executable(${exe_name} ${file})
#         get_filename_component(test_name ${file} NAME_WLE)
#         add_test(
#             NAME ${test_name}
#             COMMAND $<TARGET_FILE:${exe_name}>
#         )
#     endforeach()
# endfunction()

# cmake_language(CALL build_tests)