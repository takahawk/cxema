#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

static char* next(Tokenizer *self) 
{
	// TODO: impl
	return NULL;
}

static Tokenizer* from_string(char *str) {
	Tokenizer *tokenizer = malloc(sizeof(*tokenizer));

	*tokenizer = TOKENIZER.prototype;
	size_t len = strlen(str);
	tokenizer->str = malloc(len + 1);
	tokenizer->i = 0;

	return tokenizer;
}

static void release(Tokenizer **pself)
{
	Tokenizer *self = *pself;
	free(self->str);
	free(*pself);
	*pself = NULL;
}

const struct _TokenizerStatic TOKENIZER = {
	.prototype = {
		.next = next,
		.release = release
	},
	.from_string = from_string
};
