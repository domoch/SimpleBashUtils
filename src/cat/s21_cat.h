#include <getopt.h>
#include <stdio.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} flag_type;

int parse_flags(int argc, char *argv[], flag_type *flags);
int print_file(char *argv[], flag_type *flags);