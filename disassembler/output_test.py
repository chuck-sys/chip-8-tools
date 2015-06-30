#!/usr/bin/env python
# File: disassembler/output_test.py
# Takes 2 arguments:
# First argument is the exe (with parameters in string)
# Second argument is the file that it is comparing the output of the exe against
import subprocess
import sys

if len(sys.argv) < 3:
    print 'Error: Invalid/Missing arguments (2 required)'
    sys.exit(-1)

EXE_STRING = sys.argv[1]
FILE_NAME = sys.argv[2]

print EXE_STRING

# Run the command
output = subprocess.check_output(EXE_STRING.split(' '))

# Check the strings
model_file = open(FILE_NAME, 'r')
model_output = model_file.read()
model_file.close()

assert(output == model_output)
