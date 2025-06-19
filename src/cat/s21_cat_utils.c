#include "s21_cat.h"

void print_file(const char* filename, Flags opts) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "s21_cat: %s: Нет такого файла\n", filename);
    exit(1);
  }
  int line_num = 1, ch, prev_ch = '\n';
  while ((ch = fgetc(file)) != EOF) {
    if (opts.v && !isprint(ch) && ch != '\n' && ch != '\t') {
      if (ch == 127) {
        printf("^?");
      } else if (ch == 27) {
        printf("^[");
      } else if (ch < 32) {
        printf("^%c", ch + 64);
      } else if ((unsigned char)ch > 127) {
        unsigned char c = (unsigned char)ch;
        if (c == 255) {
          printf("M-^?");
        } else if (c >= 128 && c < 160) {
          printf("M-^%c", (c - 128) + 64);
        } else {
          printf("M-%c", c - 128);
        }
      }
      prev_ch = ch;
      continue;
    }
    if (opts.number_nonblank && ch != '\n' && prev_ch == '\n') {
      printf("%6d\t", line_num++);
    } else if (opts.number && prev_ch == '\n') {
      printf("%6d\t", line_num++);
    }
    if (opts.E && ch == '\n') putchar('$');
    if (opts.T && ch == '\t') {
      printf("^I");
      prev_ch = ch;
      continue;
    }
    if (opts.squeeze_blank && ch == '\n' && prev_ch == '\n') continue;
    putchar(ch);
    prev_ch = ch;
  }
  fclose(file);
}

void parse_args(int argc, const char** argv, Flags* opts, int* file_start,
                _Bool* is_error) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (strcmp(argv[i], "--number-nonblank") == 0) {
        opts->number_nonblank = 1;
      } else if (strcmp(argv[i], "--number") == 0) {
        opts->number = 1;
      } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
        opts->squeeze_blank = 1;
      } else {
        *is_error = true;
      }
    }
    if (argv[i][0] == '-' && argv[i][1] != '-') {
      for (int j = 1; argv[i][j] != '\0'; j++) {
        switch (argv[i][j]) {
          case 'b':
            opts->number_nonblank = 1;
            break;
          case 'E':
            opts->E = 1;
            break;
          case 'n':
            opts->number = 1;
            break;
          case 's':
            opts->squeeze_blank = 1;
            break;
          case 'T':
            opts->T = 1;
            break;
          case 'e':
            opts->E = opts->v = 1;
            break;
          case 't':
            opts->T = opts->v = 1;
            break;
          case 'v':
            opts->v = 1;
            break;
          default:
            *is_error = true;
        }
      }
    } else {
      *file_start = i;
    }
  }
}