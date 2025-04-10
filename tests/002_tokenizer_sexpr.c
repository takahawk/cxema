#include "tokenizer.h"

#include "asserts.h"

int main() {
	Tokenizer *t = TOKENIZER.from_string("(+ 137 349)");
	char *token = t->next(t);
	assert_str_equals("(", token);
	token = t->next(t);
	assert_str_equals("+", token);
	token = t->next(t);
	assert_str_equals("137", token);
	token = t->next(t);
	assert_str_equals("349", token);
	token = t->next(t);
	assert_str_equals(")", token);
	token = t->next(t);
	assert_null(token);

	free(token);
	t->release(&t);


	t = TOKENIZER.from_string("(- 1000 344)");
	token = t->next(t);
	assert_str_equals("(", token);
	token = t->next(t);
	assert_str_equals("-", token);
	token = t->next(t);
	assert_str_equals("1000", token);
	token = t->next(t);
	assert_str_equals("344", token);
	token = t->next(t);
	assert_str_equals(")", token);
	token = t->next(t);
	assert_null(token);
	free(token);
	t->release(&t);


	t = TOKENIZER.from_string("(* 5 99)");
	token = t->next(t);
	assert_str_equals("(", token);
	token = t->next(t);
	assert_str_equals("*", token);
	token = t->next(t);
	assert_str_equals("5", token);
	token = t->next(t);
	assert_str_equals("99", token);
	token = t->next(t);
	assert_str_equals(")", token);
	token = t->next(t);
	assert_null(token);
	free(token);
	t->release(&t);

	t = TOKENIZER.from_string("(* 5 99)");
	token = t->next(t);
	assert_str_equals("(", token);
	token = t->next(t);
	assert_str_equals("*", token);
	token = t->next(t);
	assert_str_equals("5", token);
	token = t->next(t);
	assert_str_equals("99", token);
	token = t->next(t);
	assert_str_equals(")", token);
	token = t->next(t);
	assert_null(token);
	free(token);
	t->release(&t);

	t = TOKENIZER.from_string("(/ 10 5)");
	token = t->next(t);
	assert_str_equals("(", token);
	token = t->next(t);
	assert_str_equals("/", token);
	token = t->next(t);
	assert_str_equals("10", token);
	token = t->next(t);
	assert_str_equals("5", token);
	token = t->next(t);
	assert_str_equals(")", token);
	token = t->next(t);
	assert_null(token);
	free(token);
	t->release(&t);

	t = TOKENIZER.from_string("(+ 2.7 10)");
	token = t->next(t);
	assert_str_equals("(", token);
	token = t->next(t);
	assert_str_equals("+", token);
	token = t->next(t);
	assert_str_equals("2.7", token);
	token = t->next(t);
	assert_str_equals("10", token);
	token = t->next(t);
	assert_str_equals(")", token);
	token = t->next(t);
	assert_null(token);
	free(token);
	t->release(&t);

	t = TOKENIZER.from_string("(+ 21 35 12 7)");
	token = t->next(t);
	assert_str_equals("(", token);
	token = t->next(t);
	assert_str_equals("+", token);
	token = t->next(t);
	assert_str_equals("21", token);
	token = t->next(t);
	assert_str_equals("35", token);
	token = t->next(t);
	assert_str_equals("12", token);
	token = t->next(t);
	assert_str_equals("7", token);
	token = t->next(t);
	assert_str_equals(")", token);
	token = t->next(t);
	assert_null(token);
	free(token);
	t->release(&t);

	t = TOKENIZER.from_string("(* 25 4 12)");
	token = t->next(t);
	assert_str_equals("(", token);
	token = t->next(t);
	assert_str_equals("*", token);
	token = t->next(t);
	assert_str_equals("25", token);
	token = t->next(t);
	assert_str_equals("4", token);
	token = t->next(t);
	assert_str_equals("12", token);
	token = t->next(t);
	assert_str_equals(")", token);
	token = t->next(t);
	assert_null(token);
	free(token);
	t->release(&t);
	return 0;
}