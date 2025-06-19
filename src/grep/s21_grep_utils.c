#include "s21_grep.h"

bool parse_args(int argc, char* argv[], Flags* opts) {
  int opt;
  bool success = true;
  while ((opt = getopt(argc, argv, "e:ivclnhs")) != -1 && success) {
    switch (opt) {
      case 'e':
        if (opts->pattern_count >= MAX_PATTERNS) {
          fprintf(stderr, "Много паттернов\n");
          success = false;
        } else {
          opts->patterns[opts->pattern_count++] = optarg;
        }
        break;
      case 'i':
        opts->i = 1;
        break;
      case 'v':
        opts->v = 1;
        break;
      case 'c':
        opts->c = 1;
        break;
      case 'l':
        opts->l = 1;
        break;
      case 'n':
        opts->n = 1;
        break;
      case 'h':
        opts->h = 1;
        break;
      case 's':
        opts->s = 1;
        break;
      default:
        success = false;
    }
  }
  if (opts->pattern_count == 0 && optind < argc) {
    opts->patterns[opts->pattern_count++] = argv[optind++];
  }
  if (success) {
    success = parse_args_file(argc, argv, opts);
  }
  return success;
}

bool parse_args_file(int argc, char* argv[], Flags* opts) {
  bool success = true;
  for (int i = optind; i < argc; i++) {
    if (opts->file_count >= MAX_FILES) {
      fprintf(stderr, "много файлов\n");
      success = false;
    } else {
      opts->files[opts->file_count++] = argv[i];
    }
  }

  if (opts->pattern_count == 0) {
    if (opts->file_count == 0) {
      fprintf(stderr, "Шаблон не указан\n");
      success = false;
    } else {
      opts->patterns[opts->pattern_count++] = opts->files[0];
      for (int i = 1; i < opts->file_count; i++) {
        opts->files[i - 1] = opts->files[i];
      }
      opts->file_count--;
    }
  }
  return success;
}

bool grep_pattern(const char* pattern, int i, regex_t* regex) {
  bool temp = true;
  int flags = REG_NOSUB;

  if (i) {
    flags |= REG_ICASE;
  }

  int reg = regcomp(regex, pattern, flags);
  if (reg != 0) {
    fprintf(stderr, "Ошибка паттерна!");
    regfree(regex);
    temp = false;
  }
  return temp;
}

bool print_file(const char* filename, const Flags* opts) {
  bool success = true;
  FILE* file = fopen(filename, "r");
  if (!file) {
    if (!opts->s) {
      perror(filename);
    }
    success = false;
  } else {
    regex_t regexes[MAX_PATTERNS];
    for (int i = 0; i < opts->pattern_count && success; i++) {
      if (!grep_pattern(opts->patterns[i], opts->i, &regexes[i])) {
        success = false;
      }
    }
    int match_count = 0;
    if (success) {
      success =
          process_file_content(file, filename, opts, regexes, &match_count);
    }

    if (opts->c && success) {
      if (opts->file_count > 1) {
        printf("%s:", filename);
      }
      printf("%d\n", match_count);
    }
    if (opts->l && match_count > 0 && success) {
      printf("%s\n", filename);
    }
    for (int i = 0; i < opts->pattern_count; i++) {
      regfree(&regexes[i]);
    }
  }
  if (file) {
    fclose(file);
  }

  return success;
}

bool process_file_content(FILE* file, const char* filename, const Flags* opts,
                          regex_t* regexes, int* match_count) {
  char line[MAX_LINE_LENGTH];
  int line_num = 0;
  bool success = true;

  while (fgets(line, sizeof(line), file) != NULL && success) {
    line_num++;
    bool matched = false;
    for (int i = 0; i < opts->pattern_count && !matched; i++) {
      if (regexec(&regexes[i], line, 0, NULL, 0) == 0) {
        matched = true;
      }
    }
    if (opts->v) {
      matched = !matched;
    }
    if (matched) {
      (*match_count)++;

      if (!opts->c && !opts->l) {
        if (!opts->h && opts->file_count > 1) {
          printf("%s:", filename);
        }
        if (opts->n) {
          printf("%d:", line_num);
        }
        if (fputs(line, stdout) == EOF) {
          perror("Ошибка ввода\n");
          success = false;
        } else {
          size_t len = strlen(line);
          if (len == 0 || line[len - 1] != '\n') {
            fputc('\n', stdout);
          }
        }
      }
    }
  }
  if (ferror(file)) {
    perror("Ошибка чтения\n");
    success = false;
  }
  return success;
}