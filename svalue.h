#ifndef SVALUE_H_
#define SVALUE_H_
#include <stdbool.h>
#include <stdint.h>

struct Env;
typedef struct Env Env;

typedef struct SValue SValue;
typedef enum SValueType SValueType;

enum SValueType {
	SVAL_TYPE_INT,
  SVAL_TYPE_FLOAT,
	SVAL_TYPE_ERR,
	SVAL_TYPE_CONS,
  SVAL_TYPE_FUNC,
  SVAL_TYPE_SYMBOL
};

typedef struct {
  SValue* (*eval) (Env *env, SValue *args, void *ctx);

  void *ctx;
} SFunction;

struct SValue {
	SValueType type;
	union {
		int64_t _int;
    double  _float;
		char *err;
		struct {
			SValue *car;
			SValue *cdr;
		} cons;
    SFunction func;
    char *symbol;
	} val;
};

struct _SValueStatic {
  SValue* (*symbol)    (const char *symbol);
  SValue* (*func)      (SValue* (*eval) (Env*, SValue*, void*), void *ctx);
	SValue* (*errorf)    (const char *fmt, ...);
	SValue* (*_int)      (int64_t _int);
  SValue* (*_float)    (double _float);
  SValue* (*cons)      (SValue *car, SValue *cdr);

	char*   (*to_string) (SValue *svalue);

  void    (*release)   (SValue **svalue);
};
extern const struct _SValueStatic SVALUE;

struct _SValueTypeStatic {
	char* (*to_string) (SValueType type);
};
extern const struct _SValueTypeStatic SVALUE_TYPE;

struct _SFunctionStatic {
  SValue* (*apply) (SFunction func, Env *env, SValue *args);
};
extern const struct _SFunctionStatic SFUNCTION;

#endif
