cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pascc LANGUAGES C CXX)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug)
endif()

enable_testing()

# external dependencies
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

message(STATUS "FLEX_EXECUTABLE: ${FLEX_EXECUTABLE}")
message(STATUS "FLEX_VERSION: ${FLEX_VERSION}")
message(STATUS "FLEX_LIBRARIES: ${FLEX_LIBRARIES}")
message(STATUS "FLEX_INCLUDE_DIRS: ${FLEX_INCLUDE_DIRS}")
message(STATUS "BISON_EXECUTABLE: ${BISON_EXECUTABLE}")
message(STATUS "BISON_VERSION: ${BISON_VERSION}")

# Sanitizer
if (NOT DEFINED PASCC_SANITIZER)
    set(PASCC_SANITIZER address)
endif()

message("Build type: ${CMAKE_BUILD_TYPE}")
message("${PASCC_SANITIZER} will be enabled in debug mode.")

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -Werror -fsanitize=${PASCC_SANITIZER} -fno-omit-frame-pointer -O0 -g")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

# output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Includes
set(PASCC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PASCC_UNITTEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/unittest/include)
set(PASCC_3RDPARTY_INCLUDE_DIR
        ${PROJECT_SOURCE_DIR}/3rdparty
        ${PROJECT_SOURCE_DIR}/3rdparty/fmt/include
        ${PROJECT_SOURCE_DIR}/3rdparty/argparse/include
)

include_directories(${PASCC_INCLUDE_DIR} ${PASCC_UNITTEST_INCLUDE_DIR} ${PASCC_3RDPARTY_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(3rdparty)
add_subdirectory(src)
add_subdirectory(unittest)
add_subdirectory(app)
