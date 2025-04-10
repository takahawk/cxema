#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void assert_str_equals(char *expected, char *actual) {
	if (actual == NULL) {
		fprintf(stderr, "assertiong failed. Expected: \"%s\", actual: NULL.\n", expected);
		exit(EXIT_FAILURE);
	}
	if (strcmp(expected, actual) != 0) {
		fprintf(stderr, "assertiong failed. Expected: \"%s\", actual: \"%s\".\n", expected, actual);
		exit(EXIT_FAILURE);
	}
}