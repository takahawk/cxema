#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "repl.h"
#include "file_interpreter.h"

bool print_top_values = false;

void print_usage()
{
  printf("Usage: cxema [FILENAME]\n");
  printf("    -t, --print-top-values   print all evaluated values on the top level\n");
}

void parse_flag(char *flag)
{ 
  if (strcmp(flag, "-t") == 0 || 
      strcmp(flag, "--print-top-values") == 0) {
    print_top_values = true;
    return;
  }

  fprintf(stderr, "unrecognized flag: %s\n", flag);
  print_usage();
}

int main(int argc, char **argv)
{
  char *filename = NULL;
  if (argc == 1) {
    return repl();
  }

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      parse_flag(argv[i]);
      continue;
    }

    if (filename) {
      fprintf(stderr, "TODO: implement intepreting multiple files\n");
      return -1;
    }
    filename = argv[i];
  }

  if (!filename) {
    print_usage();
    return -1;
  }

  return interpret_file(filename, print_top_values);
}
