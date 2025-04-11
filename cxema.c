#include "cxema.h"

#include <stdlib.h>

#include "tokenizer.h"
#include "util.h"

static SValue* _parse(Cxema *self, Tokenizer *t) {

	char *token;
	while ((token = t->next(t)) != NULL && strcmp(token, ")") != 0) {
		if (strcmp(token, "(") == 0) {
			_parse(self, t);
		}

		if (is_integer(token)) {
			long num = strtol(token, NULL, 10);
			// TODO: error handling
			return SVALUE.from_num(num);
		}

		
		// TODO: impl
		free(token);
	}
}

static SValue* parse(Cxema *self, char *code) {
	Tokenizer *t = TOKENIZER.from_string(code);
	SValue *result = _parse(self, t);
	// TODO: error if there are still tokens
	t->release(&t);
	return result;

}

static SValue* interpret(Cxema *self, char *code) {
	return parse(self, code);

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
		.parse = parse,
		.interpret = interpret,
		.release = release,
	},
	.form = form
};

