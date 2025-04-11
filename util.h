#ifndef CXEMA_UTIL_H_
#define CXEMA_UTIL_H_
#include <stdbool.h>
static inline bool one_of(char c, const char *chars) 
{
	char c2;
	for (int i = 0; (c2 = chars[i]) != '\0'; ++i) {
		if (c == c2)
			return true;
	}

	return false;
}

static inline bool is_integer(const char* token) 
{
	int i = 0;
	if (token[i] == '-')
		i++;
	char c;
	for (; (c = token[i]) != '\0'; ++i)
		if (!one_of(c, "0123456789"))
			return false;

	return true;
}

static inline bool is_decimal(const char* token) {
	int i = 0;
	if (token[i] == '-')
		i++;
	char c;
	bool has_dot = false;
	for (; (c = token[i]) != '\0'; ++i) {
		if (c == '.') {
			if (has_dot)
				return false;
			has_dot = true;
			continue;
		}
		if (!one_of(c, "0123456789"))
			return false;
	}

	return true;
}
#endif