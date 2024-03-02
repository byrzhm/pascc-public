add_library(
    pascc_driver
    OBJECT
    driver.cpp
)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:pascc_driver>
    PARENT_SCOPE)