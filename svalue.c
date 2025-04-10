#include "svalue.h"

#include <stdlib.h>

static SValue* from_num(long num) {
	SValue* value = malloc(sizeof(*value));

	value->type = SVAL_TYPE_NUM;
	value->val.num = num;

	return value;
}

static void release(SValue **svalue) {
	free(*svalue);
	*svalue = NULL;
}

const struct _SValueStatic SVALUE = {
	.from_num = from_num,
	.release  = release
};
