#include "s21_cat.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "s21_cat: не указаны файлы\n");
    return 1;
  }
  Flags opts = {0};
  int file_start = argc;
  bool is_error = false;
  parse_args(argc, (const char**)argv, &opts, &file_start, &is_error);
  if (is_error == false) {
    if (opts.number_nonblank) {
      opts.number = 0;
    }

    if (opts.e || opts.t) {
      opts.v = 1;
    }

    if (file_start >= argc) {
      fprintf(stderr, "s21_cat: ошибка файла\n");
      is_error = true;
    }

    if (is_error != true) {
      for (int i = file_start; i < argc; i++) {
        print_file(argv[i], opts);
      }
    }
  } else {
    fprintf(stderr, "s21_cat: Неизвестная опция\n");
  }

  return is_error;
}