#include "repl.h"

int main(int argc, char **argv)
{
  if (argc == 1) {
    return repl();
  }

  return -1;
}
