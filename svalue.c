#include "svalue.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static SValue* cons(SValue *)

static SValue* num(long num) {
	SValue* value = malloc(sizeof(*value));

	value->type = SVAL_TYPE_NUM;
	value->val.num = num;

	return value;
}

static SValue* errorf(const char *fmt, ...) {
	SValue *sval = malloc(sizeof(*sval));
	sval->type = SVAL_TYPE_ERR;

	va_list va;
	va_start(va, fmt);
	char *error = malloc(1024);

	vsnprintf(error, 1023, fmt, va);

	error = realloc(error, strlen(error) + 1);

	va_end(va);

	sval->val.err = error;

	return sval;
}

static char* sval_to_string(SValue *svalue) {
	// TODO: evaluate length beforehand
	char *result = malloc(1024);

	switch (svalue->type) {
	case SVAL_TYPE_NUM:
		snprintf(result, 1023, "%ld", svalue->val.num);
		break;
	case SVAL_TYPE_ERR:
		strncpy(result, svalue->val.err, 1023);
		break;
	}

	return result;
}

static void release(SValue **pself) {
	SValue *self = *pself;

	switch (self->type) {
	case SVAL_TYPE_ERR:
		free(self->val.err);
		break;
	case SVAL_TYPE_NUM:
		break;
	}

	free(self);
	*pself = NULL;
}

const struct _SValueStatic SVALUE = {
	.to_string = sval_to_string,
	.errorf    = errorf,
	.num       = num,
	.release   = release
};

static char* sval_type_to_string(SValueType type) {
	switch (type) {
	case SVAL_TYPE_NUM:
		return "Integer";
	case SVAL_TYPE_ERR:
		return "Error";
	default:
		return "Unknown";
	}
}

const struct _SValueTypeStatic SVALUE_TYPE = {
	.to_string = sval_type_to_string
};
