#include "cxema.h"

#include <stdlib.h>

static SValue* interpret(Cxema *self, char *code) {
	// TODO: implement
	return form_svalue_num(0);

}

static void release(Cxema **pself) {
	free(*pself);
	*pself = NULL;
}

const Cxema CXEMA_PROTOTYPE = {
	.interpret = interpret,

	.release = release
};

// TODO: custom allocator?
Cxema *form_cxema() {
	Cxema *cxema = malloc(sizeof(*cxema));
	*cxema = CXEMA_PROTOTYPE;
	return cxema;
}

