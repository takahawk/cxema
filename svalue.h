#ifndef SVALUE_H_
#define SVALUE_H_
#include <stdbool.h>
#include <stdint.h>

#include "special_form.h"

struct Env;
typedef struct Env Env;

typedef struct SValue SValue;

typedef enum SValueType SValueType;
enum SValueType {
	SVAL_TYPE_INT,
  SVAL_TYPE_FLOAT,
  SVAL_TYPE_BOOL,
	SVAL_TYPE_ERR,
	SVAL_TYPE_CONS,
  SVAL_TYPE_FUNC,
  SVAL_TYPE_SYMBOL,
  SVAL_TYPE_SPECIAL_FORM,
  SVAL_TYPE_VOID
};

typedef enum SpecialForm SpecialForm;
enum SpecialForm {
  SPECIAL_FORM_DEFINE,
  SPECIAL_FORM_LAMBDA,
};

typedef struct {
  bool is_builtin;
  union {
    SValue* (*builtin) (SValue *val);

    struct {
      Env *env;
      // list of symbols
      SValue *params;
      SValue *body;
    } scheme;
  } f;

} SFunction;

struct SValue {
	SValueType type;
	union {
		int64_t _int;
    double  _float;
    bool    _bool;
		char *err;
		struct {
			SValue *car;
			SValue *cdr;
		} cons;
    SFunction func;
    char *symbol;
    SpecialForm special_form;
    struct {} _void;
	} val;
};

extern const SValue SVAL_VOID;

struct _SValueStatic {
  SValue* (*symbol)       (const char *symbol);
  SValue* (*builtin_func) (SValue* (*eval) (SValue*));
  SValue* (*scheme_func)  (Env *env, SValue *params, SValue *body);
	SValue* (*errorf)       (const char *fmt, ...);
	SValue* (*_int)         (int64_t _int);
  SValue* (*_float)       (double _float);
  SValue* (*_bool)        (bool _bool);
  SValue* (*cons)         (SValue *car, SValue *cdr);
  SValue* (*special_form) (SpecialForm form);
  SValue* (*copy)         (SValue *val);

  bool    (*is_symbol)    (SValue *val);

	char*   (*to_string) (SValue *svalue);

  void    (*release)   (SValue **svalue);
};
extern const struct _SValueStatic SVALUE;

struct _SValueTypeStatic {
	char* (*to_string) (SValueType type);
};
extern const struct _SValueTypeStatic SVALUE_TYPE;

#endif
