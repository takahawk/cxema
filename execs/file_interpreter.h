#include <stdio.h>
#include <stdlib.h>

#include "codex/fs/files.h"

#include "cons.h"
#include "cxema.h"
#include "svalue.h"

int interpret_file(const char *filename) {
  char *buf = FILES.read_to_str(filename);
  if (!buf) {
    fprintf(stderr, "error reading file: %s\n", filename);
    return -1;
  }

	Cxema *cxema = CXEMA.form();

  SValue *result = cxema->interpret(cxema, buf);
  SVALUE.release(&result);

	return 0;
}
