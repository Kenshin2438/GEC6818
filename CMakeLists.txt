cmake_minimum_required(VERSION 3.28.3)
project(GEC6818 LANGUAGES C)

if(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "CMake inside source")
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wpedantic -pthread")

include(toolchain.cmake)

include_directories("${PROJECT_SOURCE_DIR}/util")
include_directories("${PROJECT_SOURCE_DIR}/src")

add_subdirectory("${PROJECT_SOURCE_DIR}/util")
add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/apps")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# target format
file (GLOB_RECURSE ALL_SRC_FILES *.h *.c)
add_custom_target (format "clang-format" -i ${ALL_SRC_FILES} COMMENT "Formatting source code...")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
