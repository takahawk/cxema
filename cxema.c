#include "cxema.h"

#include <stdlib.h>

static SValue* interpret(Cxema *self, char *code) {
	// TODO: implement
	return SVALUE.from_num(0);

}

static void release(Cxema **pself) {
	free(*pself);
	*pself = NULL;
}

const struct _CxemaStatic CXEMA = {
	.prototype = {
		.interpret = interpret,
		.release = release,
	}
};

// TODO: custom allocator?
Cxema *form_cxema() {
	Cxema *cxema = malloc(sizeof(*cxema));
	*cxema = CXEMA.prototype;
	return cxema;
}

