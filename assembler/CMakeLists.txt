set (ASSEMBLER_NAME "Chip8_Assembler")
set (ASSEMBLER_SOURCES "${PROJECT_SOURCE_DIR}/assembler/Assembler.cpp"
    "${PROJECT_SOURCE_DIR}/assembler/Parser.cpp")

add_executable (${ASSEMBLER_NAME} ${ASSEMBLER_SOURCES})
