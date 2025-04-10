#include "cxema.h"

static SValue* interpret(char *code) {
	// TODO: implement
	return form_svalue_num(0);

}

static void release(Cxema **pself) {
	free(*pself);
	*pself = NULL;
}

const Schism CXEMA_PROTOTYPE = {
	.interpret = interpret,

	.release = release
};

// TODO: custom allocator?
Cxema *form_cxema() {
	Cxema *cxema = malloc(sizeof(*cxema));
	return cxema;
}

