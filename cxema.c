#include "cxema.h"

#include <stdlib.h>
#include <errno.h>

#include "tokenizer.h"
#include "util.h"

static SValue* _parse_sexpr(Cxema *self, Tokenizer *t) {
	while ((token = t->next(t)) != NULL && strcmp(token, ")") != 0) {

	}
}


static SValue* _parse(Cxema *self, Tokenizer *t) {
	char *token;
	SValue* result = NULL;

	token-
	while ((token = t->next(t)) != NULL && strcmp(token, ")") != 0) {
		SValue *val;
		if (strcmp(token, "(") == 0) {
			// TODO: impl
			_parse_sexpr(self, t);
		}

		if (is_integer(token)) {
			long num = strtol(token, NULL, 10);
			if (errno == ERANGE) {
				return SVALUE.errorf("Value \"%s\" is too big", token);
			} else if (errno == EINVAL) {
				return SVALUE.errorf("Invalid number \"%s\"", token);
			}

			val = SVALUE.num(num);
		}

		if (NULL == result) {
			result = val;
		} else {

		}

		
		// TODO: impl
		free(token);
	}
}

static SValue* parse(Cxema *self, char *code) {
	Tokenizer *t = TOKENIZER.from_string(code);
	SValue *result = _parse(self, t);

	char *token = t->next(t);

	if (NULL != token) {
		SVALUE.release(&result);
		result = SVALUE.errorf("Unexpected token: \"%s\"", token);

	}
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

