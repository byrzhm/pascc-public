
cmake_minimum_required(VERSION 3.10)

include(GoogleTest)

file(GLOB_RECURSE PASCC_TEST_SOURCES "${PROJECT_SOURCE_DIR}/unittest/*/*test.cpp")

foreach(pascc_test_source ${PASCC_TEST_SOURCES})
    get_filename_component(pascc_test_filename ${pascc_test_source} NAME)
    string(REPLACE ".cpp" "" pascc_test_name ${pascc_test_filename})
    add_executable(${pascc_test_name} ${pascc_test_source})
    gtest_discover_tests(${pascc_test_name})
    target_link_libraries(
        ${pascc_test_name} 
        gtest_main 
        pascc_lib
        fmt
    )

    set_target_properties(
        ${pascc_test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/unittest"
        COMMAND ${pascc_test_name}
    )
endforeach()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data/codegen" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/data")