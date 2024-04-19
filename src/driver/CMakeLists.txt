add_library(
    pascc_driver
    OBJECT
    driver.cpp
)

add_dependencies(pascc_driver pascc_parse)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:pascc_driver>
    PARENT_SCOPE)