add_library(
    pascc_semant
    OBJECT
    semant.cpp
    semant_context.cpp
)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:pascc_semant>
    PARENT_SCOPE)