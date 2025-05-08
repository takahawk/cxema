#include "tokenizer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


static bool has_next(Tokenizer *self)
{
  char *str = self->str;
  int i = self->i;
skip:
  while (one_of(str[i], " \n\t")) i++;

  if (str[i] == ';') {
    // comments. skip
    char c;
    while ((c = str[i]) != '\n' && c != '\0') {
      i++;
    }

    goto skip;
  }
  self->i = i;
  return '\0' != self->str[self->i];
}

static char* next(Tokenizer *self)
{
	if (!self->has_next(self)) {
		return NULL;
	}
	char *str = self->str;
	char c;
	size_t start, i;
	i = start = self->i;

	while (!((c = str[i]) == '\0' || one_of(c, " \n\t"))) {
		i++;
		if (one_of(c, "()")) {
			// do not include if brace is not separate
			if (i - start != 1)
				i--;
			break;
		}
	}

	if (i == start)
		return NULL;

	char *token = malloc(i - start + 1);
	strncpy(token, str + start, i - start);
	token[i-start] = '\0';

	self->i = i;

	return token;
}

static Tokenizer* from_string(char *str)
{
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
    .has_next = has_next,
		.next = next,
		.release = release
	},
	.from_string = from_string
};
