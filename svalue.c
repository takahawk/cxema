#include "svalue.h"

#include <stdlib.h>

SValue* form_svalue_num(long num) {
	SValue* value = malloc(sizeof(*value));

	value->val.type = SVAL_TYPE_NUM;
	value->val.num = num;

	return value;
}

void release_svalue(Svalue **svalue) {
	free(*svalue);
	*svalue = NULL;
}
