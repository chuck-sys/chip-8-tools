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
  output: '@PLAINNAME@.yy.cc',
  arguments: ['-o', '@OUTPUT@', '@INPUT@']
)
lfiles = lgen.process('assembler/lex.l')

# bison
pgen = generator(bison,
  output: ['@BASENAME@.tab.cc', '@BASENAME@.tab.h'],
  arguments: ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@']
)
pfiles = pgen.process('assembler/grammar.y')

# Emulator
executable('emulator',
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
  pfiles,
  lfiles,
  'assembler/CmdParser.cpp',
  'assembler/Generator.cpp',
  'assembler/Utilities.cpp',
  'assembler/Assembler.cpp',
)
