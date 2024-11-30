#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int c;
  int e;
  int i;
  int l;
  int n;
  int v;
  int H;
} flag_type;

int parse_flags(int argc, char *argv[], flag_type *flags, char **pattern);
int add_pattern(char **pattern, char *str);
void proc_files(int argc, char *argv[], regex_t rgx_ptrn, flag_type flags);
void parse_single_file(char *f_name, regex_t rgx_ptrn, flag_type flags);