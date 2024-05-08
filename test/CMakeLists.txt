file(GLOB_RECURSE PASCC_PYTEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*test.py")

foreach(pascc_pytest_source ${PASCC_PYTEST_SOURCES})
    get_filename_component(pascc_pytest_name ${pascc_pytest_source} NAME_WE)
    add_test(
        NAME ${pascc_pytest_name}
        COMMAND python3 ${pascc_pytest_source} 
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endforeach()
