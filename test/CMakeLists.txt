file(GLOB_RECURSE PASCC_PYTEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*test.py")

foreach(pascc_pytest_source ${PASCC_PYTEST_SOURCES})
    get_filename_component(pascc_pytest_name ${pascc_pytest_source} NAME_WE)
    add_test(
        NAME ${pascc_pytest_name}
        COMMAND python3 ${pascc_pytest_source} 
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

endforeach()

file(COPY test_helper.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PASCC_PYTEST_SOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# copy test data
file(COPY right/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/right/)