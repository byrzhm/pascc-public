add_library(
    pascc_ast
    OBJECT
    ast.cpp
)

add_dependencies(pascc_ast pascc_parse)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:pascc_ast>
    PARENT_SCOPE)