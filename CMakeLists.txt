cmake_minimum_required (VERSION 2.6)
project (Chip8_Tools)

# Versioning
set (Chip8_Tools_VERSION_MAJOR 0)
set (Chip8_Tools_VERSION_MINOR 0)
set (Chip8_Tools_VERSION_PATCH 0)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_subdirectory ("${PROJECT_SOURCE_DIR}/emulator")
#add_subdirectory ("${PROJECT_SOURCE_DIR}/disassembler")
#add_subdirectory ("${PROJECT_SOURCE_DIR}/assembler")

#configure_file {
#    "${PROJECT_SOURCE_DIR}/Config.h.in"
#    "${PROJECT_BINARY_DIR}/Config.h"
#}

include_directories ("${PROJECT_BINARY_DIR}")
