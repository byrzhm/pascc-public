add_library(
    pascc_util
    OBJECT
    symtab.cpp
    painter.cpp
    type/type_comparator.cpp
    type/type.cpp
)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:pascc_util>
    PARENT_SCOPE)