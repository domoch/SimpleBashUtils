#include "s21_grep.h"

int main(int argc, char *argv[]) {
  int err = 0;
  flag_type flags = {0};
  char *pattern = malloc(sizeof(char));
  pattern[0] = '\0';
  regex_t rgx_ptrn;

  if (parse_flags(argc, argv, &flags, &pattern))
    err = 1;
  else if (regcomp(&rgx_ptrn, pattern, flags.i ? REG_ICASE : 0))
    err = 2;
  else {
    proc_files(argc, argv, rgx_ptrn, flags);
    regfree(&rgx_ptrn);
  }

  pattern != NULL ? free(pattern) : 0;

  return err;
}

int parse_flags(int argc, char *argv[], flag_type *flags, char **pattern) {
  int err = 0, res = 0, arg_cur = 0;

  struct option options[] = {
      {"count", no_argument, NULL, 'c'},
      {"regexp", required_argument, NULL, 'e'},
      {"ignore-case", no_argument, NULL, 'i'},
      {"files-with-matches", no_argument, NULL, 'l'},
      {"line-number", no_argument, NULL, 'n'},
      {"invert-match", no_argument, NULL, 'v'},
      {NULL, 0, NULL, 0},
  };

  while (err == 0 && ((res = getopt_long(argc, argv, "ce:ilnvhH", options,
                                         &arg_cur)) != -1)) {
    if (res == 'e') {
      flags->e = 1;
      add_pattern(pattern, optarg) ? err = 1 : 0;
    } else if (res == 'c')
      flags->c = 1;
    else if (res == 'i')
      flags->i = 1;
    else if (res == 'l')
      flags->l = 1;
    else if (res == 'n')
      flags->n = 1;
    else if (res == 'v')
      flags->v = 1;
    else if (res == 'h')
      flags->H = -1;
    else if (res == 'H' && flags->H != -1)
      flags->H = 1;
    else if (res == '?')
      err = 2;
  }

  if (flags->e == 0 && err == 0) {
    add_pattern(pattern, argv[optind]) ? err = 3 : 0;
    optind++;
  }
  return err;
}

int add_pattern(char **pattern, char *str) {
  int err = 1;
  static size_t pattern_size = 1;
  static int first = 1;
  char *tmp = NULL;
  pattern_size += strlen(str) + (first ? 0 : 2);
  if ((tmp = realloc(*pattern, pattern_size)) != NULL) {
    *pattern = tmp;
    !first ? strcat(*pattern, "\\|") : 0;
    strcat(*pattern, str);
    err = 0;
  }
  first = 0;
  return err;
}

void proc_files(int argc, char *argv[], regex_t rgx_ptrn, flag_type flags) {
  (argc - optind > 1 && flags.H != -1) ? flags.H = 1 : 0;
  while (optind < argc) {
    parse_single_file(argv[optind], rgx_ptrn, flags);
    optind++;
  }
}

void parse_single_file(char *f_name, regex_t rgx_ptrn, flag_type flags) {
  FILE *file = NULL;

  if ((file = fopen(f_name, "r")) == NULL) {
    printf("grep: %s: No such file or directory\n", f_name);
    return;
  }

  char line[4096];
  int line_number = 1, line_matches = 0;

  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    int rgx_res = regexec(&rgx_ptrn, line, 0, NULL, 0);
    line[strlen(line)] = '\n';
    int match = 0;

    if (flags.v) {
      (rgx_res == REG_NOMATCH) ? match = 1 : 0;
    } else if (rgx_res == 0)
      match = 1;

    if (!match) {
    } else if (flags.l || flags.c) {
    } else {
      (flags.H == 1) ? printf("%s:", f_name) : 0;
      (flags.n) ? printf("%d:", line_number) : 0;
      printf("%s", line);
    }
    line_matches += match;
    line_number++;
  }

  if (flags.l) {
    (line_matches != 0) ? printf("%s\n", f_name) : 0;
  } else if (flags.c) {
    (flags.H == 1) ? printf("%s:", f_name) : 0;
    printf("%d\n", line_matches);
  } else if (line_matches != 0 && !strchr(line, '\n'))
    printf("\n");
  fclose(file);
}