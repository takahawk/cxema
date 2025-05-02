#include <stdio.h>
#include <stdlib.h>

#include "cons.h"
#include "cxema.h"
#include "svalue.h"

int repl() {
	char buffer[4096];

	Cxema *cxema = CXEMA.form();
	while (1) {
		printf("cxema> ");
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
			fprintf(stderr, "Failed to read line\n");
			exit(EXIT_FAILURE);
		}

		SValue *result = cxema->interpret_all(cxema, buffer);
    CONS.list.println_items(result);
    SVALUE.release(&result);
	}

	return 0;
}
