#include "tokenizer.h"

#include "asserts.h"

int main() {
	Tokenizer *t = TOKENIZER.from_string("; comment before \n"  
                                       "   486           \n"
                                       "; comment after  \n");

	char *token = t->next(t);

	assert_str_equals("486", token, __FILE__, __LINE__);
  free(token);

	token = t->next(t);
	assert_null(token);

	free(token);
	t->release(&t);

	return 0;
}
