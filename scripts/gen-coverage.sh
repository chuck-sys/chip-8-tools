#!/usr/bin/env sh
for f in $(find . | egrep '\.cpp'); do
    gcov -n -o . $f > /dev/null
done
