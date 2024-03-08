cmake_minimum_required(VERSION 3.28.3)
project(GEC6818 LANGUAGES C)

include(toolchain.cmake)

include_directories("${PROJECT_SOURCE_DIR}/util")

add_subdirectory("${PROJECT_SOURCE_DIR}/apps")
add_subdirectory("${PROJECT_SOURCE_DIR}/util")

# target format
file (GLOB_RECURSE ALL_SRC_FILES *.h *.c)
add_custom_target (format "clang-format" -i ${ALL_SRC_FILES} COMMENT "Formatting source code...")
