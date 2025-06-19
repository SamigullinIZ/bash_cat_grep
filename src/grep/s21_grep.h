#ifndef SRC_GREP_S21_GREP_H
#define SRC_GREP_S21_GREP_H

#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 4024
#define MAX_PATTERNS 16
#define MAX_FILES 16

typedef struct {
  int i;  // ignore
  int v;  // invert match
  int c;  // count only
  int l;  // file witch match
  int n;  // line numbers
  int h;
  int s;
  char* patterns[MAX_PATTERNS];
  int pattern_count;
  char* files[MAX_FILES];
  int file_count;
} Flags;

bool parse_args(int argc, char* argv[], Flags* opts);
bool grep_pattern(const char* pattern, int i, regex_t* regex);
bool print_file(const char* filename, const Flags* opts);
bool process_file_content(FILE* file, const char* filename, const Flags* opts,
                          regex_t* regexes, int* match_count);
bool parse_args_file(int argc, char* argv[], Flags* opts);

#endif