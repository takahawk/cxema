#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cxema.h"
#include "svalue.h"

static void assert_long_equals(long expected, long actual)
{
	if (expected != actual) {
		fprintf(stderr, "assertion failed. Expected: \"%ld\", actual: \"%ld\".\n", expected, actual);
		exit(EXIT_FAILURE);
	}

}

static void assert_type_equals(SValueType expected_type, SValue* val,
                               const char *file, int line)
{
	if (expected_type != val->type) {
		// TODO: print string types
		fprintf(stderr, "\"%s:%d\" assertion failed. Expected: \"%s\", actual: \"%s\".\n",
                    file, line,
                    SVALUE_TYPE.to_string(expected_type),
			              SVALUE_TYPE.to_string(val->type));
    if (val->type == SVAL_TYPE_ERR) {
      fprintf(stderr, "Error is \"%s\"\n", val->val.err);
    }
		exit(EXIT_FAILURE);
	}
}

static void assert_null(void *value)
{
	if (NULL != value) {
		fprintf(stderr, "value is expected to be NULL. String value=\"%s\"\n", (char *) value);
		exit(EXIT_FAILURE);
	}
}


static void assert_str_equals(char *expected, char *actual,
                              const char *file, int line)
{
	if (actual == NULL) {
		fprintf(stderr, "\"%s:%d\" assertion failed. Expected: \"%s\", actual: NULL.\n",
                    file, line,
                    expected);
		exit(EXIT_FAILURE);
	}
	if (strcmp(expected, actual) != 0) {
		fprintf(stderr, "\"%s:%d\" assertion failed. Expected: \"%s\", actual: \"%s\".\n",
                    file, line,
                    expected, actual);
		exit(EXIT_FAILURE);
	}
}

static void assert_interprets_as(Cxema *cx, const char *code,
                                 SValueType expected_type,
                                 const char *expected_str,
                                 const char *file,
                                 int line)
{
	SValue *sval = cx->interpret(cx, code);
  char *str = SVALUE.to_string(sval);
	assert_type_equals(expected_type, sval, file, line);
	assert_str_equals(expected_str, str, file, line);
  free(str);
  SVALUE.release(&sval);
}
