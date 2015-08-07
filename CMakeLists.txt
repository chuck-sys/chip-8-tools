cmake_minimum_required (VERSION 2.6)
project (Chip8_Tools)

# Versioning
set (Chip8_Tools_VERSION_MAJOR 1)
set (Chip8_Tools_VERSION_MINOR 1)
set (Chip8_Tools_VERSION_PATCH 0)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Os -Wall")

add_subdirectory ("${PROJECT_SOURCE_DIR}/emulator")
add_subdirectory ("${PROJECT_SOURCE_DIR}/disassembler")
add_subdirectory ("${PROJECT_SOURCE_DIR}/assembler")

include_directories ("${PROJECT_BINARY_DIR}")
