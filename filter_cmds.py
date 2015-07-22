#!/usr/bin/env python
# File: filter_cmds.py
# Filters commands/opcodes outputted by the disassembler to create a list
# of supposedly usable/functioning opcodes.
import sys

s = []

for l in sys.stdin:
    mnemonic = l.split('\t')[0].rstrip()
    if mnemonic.isalpha():
        s.append(mnemonic)

s = set(s)
for i in s:
    print i

print "\nTotal: %d types" % len(s)
