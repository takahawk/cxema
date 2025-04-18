#include "tokenizer.h"

#include "asserts.h"

int main() {
	Tokenizer *t = TOKENIZER.from_string("   486   ");

	char *token = t->next(t);

	assert_str_equals("486", token, __FILE__, __LINE__);

	token = t->next(t);
	assert_null(token);

	free(token);
	t->release(&t);

	return 0;
}
