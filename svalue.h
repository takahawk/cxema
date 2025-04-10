#ifndef SVALUE_H_
#define SVALUE_H_

typedef struct SValue SValue;
typedef enum SValueType SValueType;

enum SValueType {
	SVAL_TYPE_NUM,
};


struct SValue {
	SValueType type;
	union {
		long num;
	} val;
};

SValue *form_svalue_num(long num);
void release_svalue(SValue **svalue);

#endif