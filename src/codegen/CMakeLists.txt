add_library(
    pascc_codegen
    OBJECT
    codegen.cpp
    codegen_context.cpp
)

add_dependencies(pascc_codegen pascc_parse)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:pascc_codegen>
    PARENT_SCOPE)