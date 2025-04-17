#ifndef SVALUE_H_
#define SVALUE_H_

typedef struct SValue SValue;
typedef enum SValueType SValueType;

enum SValueType {
	SVAL_TYPE_NUM,
	SVAL_TYPE_ERR,
	SVAL_TYPE_CONS,
  SVAL_TYPE_FUNC,
};

typedef struct {
  // TODO:  add environment
  SValue* (*eval) (SValue *args);

  void *ctx;
} SFunction;

struct SValue {
	SValueType type;
	union {
		long num;
		char *err;
		struct {
			SValue *car;
			SValue *cdr;
		} cons;
    SFunction func;
	} val;
};

struct _SValueStatic {
	SValue* (*errorf)    (const char *fmt, ...);
	SValue* (*num)       (long num);
  SValue* (*cons)      (SValue *car, SValue *cdr);
	char*   (*to_string) (SValue *svalue);
	void    (*release)   (SValue **svalue);
};
extern const struct _SValueStatic SVALUE;

struct _SValueTypeStatic {
	char* (*to_string) (SValueType type);
};
extern const struct _SValueTypeStatic SVALUE_TYPE;

#endif
