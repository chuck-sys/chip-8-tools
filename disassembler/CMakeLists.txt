set(DISASSEMBLER_NAME "Chip8_Disassembler")
set(DISASSEMBLER_SOURCES "${PROJECT_SOURCE_DIR}/disassembler/Disassembler.cpp")

add_executable(${DISASSEMBLER_NAME} ${DISASSEMBLER_SOURCES})

# See if it runs
file(GLOB c8s "${PROJECT_SOURCE_DIR}/c8games/*")
foreach (bin ${c8s})
    get_filename_component(base ${bin} NAME)
    add_test(NAME Disassemble${base}
             COMMAND ${PROJECT_BINARY_DIR}/${DISASSEMBLER_NAME}
                     ${bin})
endforeach()
