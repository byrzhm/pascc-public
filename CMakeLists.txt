cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pascc LANGUAGES C CXX)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(CTest)

enable_testing()

#########################################################
#################   submodules update   #################
######################################################### 

find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    message(STATUS "Git found, updating submodules")

    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT
    )

    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
else()
    message(WARNING "Git not found or project is not a Git repository. Submodules will not be updated.")
endif()



#########################################################
################# external dependencies #################
#########################################################

# On macOS, search Homebrew for keg-only versions of Bison and Flex. Xcode does
# not provide new enough versions for us to use.
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    execute_process(
        COMMAND brew --prefix bison
        RESULT_VARIABLE BREW_BISON
        OUTPUT_VARIABLE BREW_BISON_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_BISON EQUAL 0 AND EXISTS "${BREW_BISON_PREFIX}")
        message(STATUS "Found Bison keg installed by Homebrew at ${BREW_BISON_PREFIX}")
        set(BISON_EXECUTABLE "${BREW_BISON_PREFIX}/bin/bison")
    endif()

    execute_process(
        COMMAND brew --prefix flex
        RESULT_VARIABLE BREW_FLEX
        OUTPUT_VARIABLE BREW_FLEX_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_FLEX EQUAL 0 AND EXISTS "${BREW_FLEX_PREFIX}")
        message(STATUS "Found Flex keg installed by Homebrew at ${BREW_FLEX_PREFIX}")
        set(FLEX_EXECUTABLE "${BREW_FLEX_PREFIX}/bin/flex")
    endif()
endif()

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

message(STATUS "FLEX_EXECUTABLE: ${FLEX_EXECUTABLE}")
message(STATUS "FLEX_VERSION: ${FLEX_VERSION}")
message(STATUS "FLEX_LIBRARIES: ${FLEX_LIBRARIES}")
message(STATUS "FLEX_INCLUDE_DIRS: ${FLEX_INCLUDE_DIRS}")
message(STATUS "BISON_EXECUTABLE: ${BISON_EXECUTABLE}")
message(STATUS "BISON_VERSION: ${BISON_VERSION}")

# 生成文档
find_package(Doxygen) # 生成文档还需要安装 dot (graphviz)

# 检查是否找到 Doxygen
if(DOXYGEN_FOUND)
    message("Doxygen found: ${DOXYGEN_EXECUTABLE}")
    message("Doxygen version: ${DOXYGEN_VERSION}")

    set(DOXYGEN_IN "${PROJECT_SOURCE_DIR}/docs/Doxyfile.in")
    set(DOXYGEN_OUT "${PROJECT_BINARY_DIR}/docs/Doxyfile")

    # Configure the Doxyfile
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Add a target to generate the documentation
    add_custom_target(doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else(DOXYGEN_FOUND)
    message("Doxygen not found, cannot generate documentation")
endif(DOXYGEN_FOUND)

#########################################################
#################   about compiler      #################
#########################################################

# Sanitizer
if (NOT DEFINED PASCC_SANITIZER)
    set(PASCC_SANITIZER address)
endif()

message("Build type: ${CMAKE_BUILD_TYPE}")
message("${PASCC_SANITIZER} will be enabled in debug mode.")

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -Werror -fsanitize=${PASCC_SANITIZER} -fno-omit-frame-pointer -O0 -g")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#
# 如果想要编译的程序在旧的glibc版本上运行, 需要添加
#       -static-libstdc++ -static-libgcc
# 在相关联的 targets 上使用 target_link_libraries
# 例如 target_link_libraries(pascc -static-libstdc++ -static-libgcc)
# 
# 或者使用全局的配置
#       -static
# 在这个位置添加 set(CMAKE_EXE_LINKER_FLAGS "-static")
# 

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
set(PASCC_3RDPARTY_INCLUDE_DIR
        ${PROJECT_SOURCE_DIR}/3rdparty
        ${PROJECT_SOURCE_DIR}/3rdparty/fmt/include
        ${PROJECT_SOURCE_DIR}/3rdparty/argparse/include
)

include_directories(
    ${PASCC_INCLUDE_DIR} 
    ${PASCC_3RDPARTY_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}/src/parse)


# 在生成文件目录中添加 clang-tidy 文件, 来取消所有检查
file(WRITE ${PROJECT_BINARY_DIR}/.clang-tidy "Checks: '-*'")

add_subdirectory(3rdparty)
add_subdirectory(src)
add_subdirectory(unittest)
add_subdirectory(app)
add_subdirectory(test)
