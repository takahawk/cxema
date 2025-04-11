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

struct _SValueStatic {
	SValue* (*from_num) (long num); 
	void    (*release)  (SValue **svalue);
};
extern const struct _SValueStatic SVALUE;

struct _SValueTypeStatic {
	char* (*to_string) (SValueType type);
};
extern const struct _SValueTypeStatic SVALUE_TYPE;

#endif