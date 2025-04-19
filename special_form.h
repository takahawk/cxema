#ifndef CXEMA_SPECIAL_FORMS_H_
#define CXEMA_SPECIAL_FORMS_H_

typedef struct SValue SValue;
typedef struct Cxema Cxema;

struct _SpecialFormsStatic {
  SValue* (*define) (Cxema *cx, SValue *args);
};

extern const struct _SpecialFormsStatic SPECIAL_FORMS;
#endif
