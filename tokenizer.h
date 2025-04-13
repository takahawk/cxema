#ifndef TOKENIZER_H_
#define TOKENIZER_H_
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

typedef struct Tokenizer Tokenizer;

struct Tokenizer {
	char *str;
	size_t i;

  bool  (*has_next) (Tokenizer *self);
	char* (*next)     (Tokenizer *self);
	void  (*release)  (Tokenizer **pself);
};

struct _TokenizerStatic {
	Tokenizer prototype;
	Tokenizer* (*from_string) (char *str);
};

extern const struct _TokenizerStatic TOKENIZER;

#endif
