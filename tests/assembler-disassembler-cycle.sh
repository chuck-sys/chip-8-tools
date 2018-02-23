#!/usr/bin/env sh

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
ASMBIN="${ROOTDIR}build/Chip8_Assembler"
# Disassembler binary
DASMBIN="${ROOTDIR}build/Chip8_Disassembler"

# Create a temporary directory to store everything
TMPDIR="$(mktemp -d)/"

for f in ${BINS} ; do
    filename="$(basename $f)"
    tmpasm="${TMPDIR}${filename}.asm"
    tmpbin="${TMPDIR}${filename}"

    echo "Doing $f..."

    # Disassemle it
    ${DASMBIN} -c $f > ${tmpasm}
    # Assemble it
    ${ASMBIN} -o ${tmpbin} ${tmpasm}

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
        exit -1
    fi
done

# Delete everything in the temporary directory
rm -rf $TMPDIR
