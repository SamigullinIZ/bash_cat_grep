#include "s21_grep.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "s21_grep: не указаны файлы\n");
    return EXIT_FAILURE;
  }
  Flags opts = {0};
  bool success = true;

  if (!parse_args(argc, argv, &opts)) {
    fprintf(stderr, "Ошибка в аргументах\n");
    success = false;
  } else {
    for (int i = 0; i < opts.file_count && success; i++) {
      if (!print_file(opts.files[i], &opts)) {
        success = false;
      }
    }
  }

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}