#!/bin/bash

MY_CAT="../cat/s21_cat"
ORIG_CAT="cat"

TEST_FILES=(
    "test_files/hello.txt"
    "test_files/empty.txt"
    "test_files/multiline.txt"
    "test_files/special_chars.txt"
)

FLAGS=(
    "-b"    # нумерует только непустые строки
    "-n"    # нумерует все строки
    "-s"    # сжимает несколько пустых строк в одну
    "-e"    # показывает символы конца строки ($) + -v
    "-t"    # показывает табы (^I) + -v
    "-E"    # показывает символы конца строки ($)
    "-T"    # показывает табы (^I)
    "-b -e"
    "-n -T"
    "-s -E -T"
)

for file in "${TEST_FILES[@]}"; do
    for flag in "${FLAGS[@]}"; do
        echo "Testing file: $file with flags: $flag"

        $MY_CAT $flag "$file" > my_output.txt 2>&1
        $ORIG_CAT $flag "$file" > orig_output.txt 2>&1

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