#ifndef SRC_CAT_S21_CAT_H
#define SRC_CAT_S21_CAT_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int number_nonblank;  // -b
  int number;           // -n
  int squeeze_blank;    // -s
  int e;
  int t;
  int E;
  int T;
  int v;
} Flags;

void print_file(const char* filename, Flags opts);
void parse_args(int argc, const char** argv, Flags* opts, int* file_start,
                _Bool* is_error);

#endif