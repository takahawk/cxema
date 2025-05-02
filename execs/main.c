#include <stdio.h>

#include "repl.h"
#include "file_interpreter.h"


int main(int argc, char **argv)
{
  if (argc == 1) {
    return repl();
  }

  if (argc != 2) {
    printf("Usage: cxema [FILENAME]\n");
    return -1;
  }

  return interpret_file(argv[1]);
}
