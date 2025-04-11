#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svalue.h"

static void assert_long_equals(long expected, long actual) {
	if (expected != actual) {
		fprintf(stderr, "assertion failed. Expected: \"%ld\", actual: \"%ld\".\n", expected, actual);
		exit(EXIT_FAILURE);
	}

}

static void assert_type_equals(SValueType expected, SValueType actual) {
	if (expected != actual) {
		// TODO: print string types
		fprintf(stderr, "assertion failed. Expected: \"%s\", actual: \"%s\".\n", 
			            SVALUE_TYPE.to_string(expected), 
			            SVALUE_TYPE.to_string(actual));
		exit(EXIT_FAILURE);
	}
}

static void assert_null(void *value) {
	if (NULL != value) {
		fprintf(stderr, "value is expected to be NULL. String value=\"%s\"\n", (char *) value);
		exit(EXIT_FAILURE);
	}
}


static void assert_str_equals(char *expected, char *actual) {
	if (actual == NULL) {
		fprintf(stderr, "assertion failed. Expected: \"%s\", actual: NULL.\n", expected);
		exit(EXIT_FAILURE);
	}
	if (strcmp(expected, actual) != 0) {
		fprintf(stderr, "assertion failed. Expected: \"%s\", actual: \"%s\".\n", expected, actual);
		exit(EXIT_FAILURE);
	}
}