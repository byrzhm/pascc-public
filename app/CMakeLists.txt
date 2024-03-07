add_executable(pascc main.cpp)

target_link_libraries(pascc
    PRIVATE
        pascc_lib
)