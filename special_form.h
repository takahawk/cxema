#ifndef CXEMA_SPECIAL_FORMS_H_
#define CXEMA_SPECIAL_FORMS_H_
#include <stdbool.h>

typedef struct SValue SValue;
typedef struct Cxema Cxema;

struct _SpecialFormsStatic {
  SValue* (*apply)            (SValue *sform, Cxema *cx, SValue *args);
  bool    (*is_special_form)  (const char *token);
  SValue* (*from_string)      (const char *token);
  SValue* (*define)           (Cxema *cx, SValue *args);
};

extern const struct _SpecialFormsStatic SPECIAL_FORMS;
#endif
