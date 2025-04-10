#include "tokenizer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static inline bool one_of(char c, const char *chars) {
	char c2;
	for (int i = 0; (c = chars[i]) != '\0'; ++i) {
		if (c == c2)
			return true;
	}

	return false;
}

static char* next(Tokenizer *self) 
{
	if ('\0' == self->str[self->i]) {
		return NULL;
	}
	char *str = self->str;
	char c;
	size_t start, i;
	start = self->i;
	while (one_of(str[start], " \n\t")) start++;

	i = start;

	while (!((c = str[i]) == '\0' || one_of(c, " \n\t"))) {
		i++;
		if (one_of(c, "()")) {
			break;
		}
	}

	char *token = malloc(i - start);
	strncpy(token, str + start, i - start);
	token[i-start] = '\0';

	self->i = i;

	return token;
}

static Tokenizer* from_string(char *str) {
	Tokenizer *tokenizer = malloc(sizeof(*tokenizer));

	*tokenizer = TOKENIZER.prototype;
	size_t len = strlen(str);
	tokenizer->str = malloc(len + 1);
	strcpy(tokenizer->str, str);
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
