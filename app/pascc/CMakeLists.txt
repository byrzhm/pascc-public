add_executable(pascc pascc.cpp)

target_link_libraries(pascc
    PRIVATE
        pascc_lib
)