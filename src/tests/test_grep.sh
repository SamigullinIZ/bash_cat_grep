#!/bin/bash

MY_GREP="../grep/s21_grep"
ORIG_GREP="grep"
PATTERN="Hello"

TEST_FILES=(
    "test_files/hello.txt"
    "test_files/empty.txt"
    "test_files/multiline.txt"
    "test_files/special_chars.txt"
)

FLAGS=(
    "-e"
    "-i"
    "-v"
    "-c"
    "-l"
    "-n"
    "-h"
    "-s"
)


for file in "${TEST_FILES[@]}"; do
    for flag in "${FLAGS[@]}"; do
        echo "Testing file: $file with flags: $flag"

        $MY_GREP $flag "$PATTERN" "$file" > my_output.txt 2>&1
        $ORIG_GREP $flag "$PATTERN" "$file" > orig_output.txt 2>&1

        if diff my_output.txt orig_output.txt; then
            echo "✅ PASSED: $flag"
        else
            echo "❌ FAILED: $flag (differences found)"
            echo "--- Diff ---"
            diff -u my_output.txt orig_output.txt | tail -n 10
            echo "------------"
        fi

        rm -f my_output.txt orig_output.txt
    done
done