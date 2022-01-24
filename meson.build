project('chip-8-tools', 'cpp')

# Dependencies
sfml_dep = dependency('sfml')
flex = find_program('flex')
bison = find_program('bison')

if not flex.found()
  error('MESSON_SKIP_TEST flex not found.')
endif

if not bison.found()
  error('MESSON_SKIP_TEST bison not found.')
endif

# flex
lgen = generator(flex,
  output: '@PLAINNAME@.yy.c',
  arguments: ['-o', '@OUTPUT@', '@INPUT@']
)

# Emulator
executable('emulator',
  'emulator/Chip_8.h',
  'emulator/Chip_8.cpp',
  'emulator/Emulator.cpp',
  dependencies: sfml_dep
)

# Disassembler
executable('disassembler',
  'disassembler/Disassembler.cpp'
)

# Assembler
executable('assembler',
  'assembler/Parser.h',
  'assembler/Parser.cpp',
  'assembler/CmdParser.h',
  'assembler/Generator.h',
  'assembler/Utilities.h',
  'assembler/Assembler.cpp',
  'assembler/CmdParser.cpp',
  'assembler/Generator.cpp',
  'assembler/Utilities.cpp',
)
