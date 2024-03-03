message(STATUS "Bison Flex will generate parser and lexer files: ")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

# 生成文件: parser.cpp, location.hh
# 生成路径: ${CMAKE_CURRENT_BINARY_DIR} 即 build/src/parse/
BISON_TARGET(PASCC_PARSER parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)

# 生成文件: lexer.cpp
# 生成路径: ${CMAKE_CURRENT_BINARY_DIR} 即 build/src/parse/
FLEX_TARGET(PASCC_LEXER lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

# 生成文件: parser.hpp
ADD_FLEX_BISON_DEPENDENCY(PASCC_LEXER PASCC_PARSER)

add_library(pascc_parse ${BISON_PASCC_PARSER_OUTPUTS} ${FLEX_PASCC_LEXER_OUTPUTS})

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:pascc_parse>
    PARENT_SCOPE)

# 在生成文件目录中添加 clang-tidy 文件, 来取消所有检查
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.clang-tidy "Checks: '-*'")
