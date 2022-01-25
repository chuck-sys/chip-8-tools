#!/usr/bin/env bash

# Sanity checking
if [[ $# -ne 1 ]]; then
    echo "Error: incorrect number of arguments."
    echo "Usage: $0 <project-directory>"
    exit -1
fi

# Root directory of the project, WITH SLASH
ROOTDIR="$1"
# Binaries
BINS="${ROOTDIR}c8games/*"
# Assembler binary
ASMBIN="${ROOTDIR}build/assembler"
# Disassembler binary
DASMBIN="${ROOTDIR}build/disassembler"

# More sanity checking
for f in ${ASMBIN} ${DASMBIN} ; do
    if [[ ! -e $f ]]; then
        echo "$f does not exist. Exiting"
        exit -1
    fi
done

# Create a temporary directory to store everything
TMPDIR="$(mktemp -d)/"
# Variable to update when any tests fail
TESTFAIL=false

for f in ${BINS} ; do
    filename="$(basename $f)"
    tmpasm="${TMPDIR}${filename}.asm"
    tmpbin="${TMPDIR}${filename}"

    echo "Doing $f..."

    # Disassemle it
    ${DASMBIN} -c $f > ${tmpasm}
    if [[ $? -ne 0 ]]; then
        echo "Could not write to ${tmpasm}. Exiting"
        TESTFAIL=true
    fi

    # Assemble it
    ${ASMBIN} -o ${tmpbin} ${tmpasm}
    if [[ $? -ne 0 ]]; then
        echo "Could not write to ${tmpbin}. Exiting"
        TESTFAIL=true
    fi

    # Compare to the original
    cmp -l $f ${tmpbin}
    if [[ $? -ne 0 ]]; then
        echo "$f and $tmpbin don't match. Exiting"
        echo "Contents of $f"
        echo "=============="
        xxd $f
        echo "Contents of $tmpbin"
        echo "==================="
        xxd $tmpbin
        TESTFAIL=true
    fi
done

# Delete everything in the temporary directory
rm -rf $TMPDIR

# Report on tests failing
if [[ ${TESTFAIL} ]]; then
    exit -1
else
    exit 0
fi
