#include <stdio.h>
#include <stdlib.h>

#include "cons.h"
#include "cxema.h"
#include "svalue.h"

int interpret_file(const char *filename, bool print_top_values) 
{
	Cxema *cxema = CXEMA.form();

  SValue *result = cxema->interpret_file_all(cxema, filename);
  if (SVALUE.is_err(result)) {
    fprintf(stderr, "%s\n", SVALUE.to_string(result));
    SVALUE.release(&result);
    return -1;
  }

  if (print_top_values) {
    SValue* head = result;
    while (head) {
      SValue *car = CONS.car(head);
      if (!SVALUE.is_void(car)) {
        printf("%s\n", SVALUE.to_string(car));
      }
      head = CONS.cdr(head);
    }
  }
  SVALUE.release(&result);

	return 0;
}
