add_library(
    pascc_util
    OBJECT
    symtab.cpp
    painter.cpp
    type/type_comparator.cpp
    type/type.cpp
)

add_dependencies(pascc_util pascc_parse)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:pascc_util>
    PARENT_SCOPE)