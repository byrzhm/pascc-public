add_subdirectory(ast)
add_subdirectory(parse)
add_subdirectory(semant)
add_subdirectory(codegen)
add_subdirectory(driver)
add_subdirectory(util)

add_library(pascc_lib STATIC ${ALL_OBJECT_FILES})

target_link_libraries(
    pascc_lib
    PRIVATE
        fmt
)