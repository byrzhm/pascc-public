# don't override our compiler/linker options when building gtest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)

add_subdirectory(fmt)

add_subdirectory(argparse)