set(DISASSEMBLER_NAME "Chip8_Disassembler")
set(DISASSEMBLER_SOURCES "${PROJECT_SOURCE_DIR}/disassembler/Disassembler.cpp")

add_executable(${DISASSEMBLER_NAME} ${DISASSEMBLER_SOURCES})

# See if it runs
add_test(DisassemblerRun
         "${PROJECT_BINARY_DIR}/${DISASSEMBLER_NAME}"
         "${PROJECT_SOURCE_DIR}/c8games/PONG")
