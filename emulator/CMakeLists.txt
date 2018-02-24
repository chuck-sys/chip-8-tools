set(EMULATOR_NAME "Chip8_Emulator")
set(EMULATOR_SOURCES "${PROJECT_SOURCE_DIR}/emulator/Chip_8.cpp"
    "${PROJECT_SOURCE_DIR}/emulator/Chip_8.h"
    "${PROJECT_SOURCE_DIR}/emulator/Emulator.cpp")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML REQUIRED system window graphics audio)

if (SFML_FOUND)
    add_executable(${EMULATOR_NAME} ${EMULATOR_SOURCES})
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EMULATOR_NAME} ${SFML_LIBRARIES})
endif ()
