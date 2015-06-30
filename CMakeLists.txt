cmake_minimum_required (VERSION 2.6)
project (Chip8_Tools)
enable_testing ()

# Versioning
set (Chip8_Tools_VERSION_MAJOR 0)
set (Chip8_Tools_VERSION_MINOR 0)
set (Chip8_Tools_VERSION_PATCH 0)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_subdirectory ("${PROJECT_SOURCE_DIR}/emulator")
add_subdirectory ("${PROJECT_SOURCE_DIR}/disassembler")

include_directories ("${PROJECT_BINARY_DIR}")
