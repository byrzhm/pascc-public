add_executable(painter ast_painter.cpp)

target_link_libraries(painter
    PRIVATE
        pascc_lib
)