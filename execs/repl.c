#include <stdio.h>
#include <stdlib.h>

#include "cxema.h"
#include "svalue.h"

int main() {
	char buffer[4096];

	Cxema *cxema = CXEMA.form();
	while (1) {
		printf("cxema> ");
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
			fprintf(stderr, "Failed to read line\n");
			exit(EXIT_FAILURE);
		}

		SValue *result = cxema->interpret(cxema, buffer);
		char *resstr = SVALUE.to_string(result);
		printf("%s\n", resstr);
		SVALUE.release(&result);
	}

	return 0;
}