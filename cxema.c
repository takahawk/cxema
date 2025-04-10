#include "cxema.h"

#include <stdlib.h>

#include "tokenizer.h"

static SValue* interpret(Cxema *self, char *code) {
	Tokenizer *t = TOKENIZER.from_string(code);

	char *token;
	while ((token = t->next(t)) != NULL) {
		// TODO: impl
		free(token);
	}
	return SVALUE.from_num(0);

}

static void release(Cxema **pself) {
	free(*pself);
	*pself = NULL;
}

static Cxema* form() {
	Cxema *cxema = malloc(sizeof(*cxema));
	*cxema = CXEMA.prototype;
	return cxema;
}

const struct _CxemaStatic CXEMA = {
	.prototype = {
		.interpret = interpret,
		.release = release,
	},
	.form = form
};

