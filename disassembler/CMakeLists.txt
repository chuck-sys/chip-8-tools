set (DISASSEMBLER_NAME "Chip8_Disassembler")
set (DISASSEMBLER_SOURCES "${PROJECT_SOURCE_DIR}/disassembler/Disassembler.cpp")

add_executable (${DISASSEMBLER_NAME} ${DISASSEMBLER_SOURCES})

# Some helpful functions
function (CreateTest_NoFlags FN)
    add_test (DisassemblerNoFlags_${FN} "${PROJECT_SOURCE_DIR}/disassembler/output_test.py"
        "${PROJECT_BINARY_DIR}/${DISASSEMBLER_NAME} ${PROJECT_SOURCE_DIR}/c8games/${FN}"
        "${PROJECT_SOURCE_DIR}/test_outputs/dis_${FN}.out")
endfunction (CreateTest_NoFlags)

# See if it runs
add_test (DisassemblerRun "${PROJECT_BINARY_DIR}/${DISASSEMBLER_NAME}" "${PROJECT_SOURCE_DIR}/c8games/PONG")
# Run some tests
CreateTest_NoFlags ("15PUZZLE")
CreateTest_NoFlags ("BLINKY")
CreateTest_NoFlags ("BLITZ")
CreateTest_NoFlags ("BRIX")
CreateTest_NoFlags ("PONG")
CreateTest_NoFlags ("PONG2")
CreateTest_NoFlags ("TANK")
