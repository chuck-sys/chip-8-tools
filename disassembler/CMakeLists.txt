set (DISASSEMBLER_NAME "Chip8_Disassembler")
set (DISASSEMBLER_SOURCES "${PROJECT_SOURCE_DIR}/disassembler/Disassembler.cpp")

add_executable (${DISASSEMBLER_NAME} ${DISASSEMBLER_SOURCES})
