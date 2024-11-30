#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int rtrn = 0;
  flag_type flags = {0};

  if (parse_flags(argc, argv, &flags)) {
    while (optind < argc) {
      if ((rtrn = print_file(argv, &flags)) == 0)
        printf("s21_cat: %s: No such file or directory\n", argv[optind]);
      optind++;
    }
  }
  return rtrn;
}

int parse_flags(int argc, char *argv[], flag_type *flags) {
  int rtrn = 1, res = 0, arg_cur = 0;
  struct option options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {NULL, 0, NULL, 0},
  };
  while (rtrn != 0 && ((res = getopt_long(argc, argv, "beEnstTv", options,
                                          &arg_cur)) != -1)) {
    if (res == 'b')
      flags->b = 1, flags->n = 0;
    else if (res == 'e')
      flags->e = 1, flags->v = 1;
    else if (res == 'E')
      flags->e = 1;
    else if (res == 'n' && flags->b != 1)
      flags->n = 1;
    else if (res == 's')
      flags->s = 1;
    else if (res == 't')
      flags->t = 1, flags->v = 1;
    else if (res == 'T')
      flags->t = 1;
    else if (res == 'v')
      flags->v = 1;
    else if (res == '?')
      rtrn = 0;
  }
  return rtrn;
};

int print_file(char *argv[], flag_type *flags) {
  int rtrn = 0;
  FILE *file_cur = NULL;
  if ((file_cur = fopen(argv[optind], "r")) != NULL) {
    static int line_index = 1, char_last = '\n', char_cur;
    rtrn = 1;
    while ((char_cur = fgetc(file_cur)) != EOF) {
      if (char_last == '\n') {
        if (char_cur == '\n' && flags->s) {
          while ((char_cur = fgetc(file_cur)) == '\n' && char_cur != EOF) {
          }
          if (char_cur != EOF) fseek(file_cur, -1, SEEK_CUR);
          char_cur = '\n';
        }
        if (flags->n)
          printf("%6d\t", line_index++);
        else if (flags->b)
          (char_cur != '\n') ? printf("%6d\t", line_index++) : (0);
      }
      if (char_cur == '\n' && flags->e)
        printf("$");
      else if ((char_cur == '\t' && flags->t) ||
               (char_cur < 32 && char_cur != 9 && char_cur != 10 && flags->v)) {
        printf("^");
        char_cur += 64;
      } else if (char_cur == 127 && flags->v) {
        printf("^");
        char_cur = 63;
      }
      char_last = char_cur;
      printf("%c", char_cur);
    }
    fclose(file_cur);
  }
  return rtrn;
}