#ifndef CXEMA_SPECIAL_FORMS_H_
#define CXEMA_SPECIAL_FORMS_H_
#include <stdbool.h>

#include "evaluator.h"

typedef struct Env Env;
typedef struct SValue SValue;
typedef struct Cxema Cxema;

struct _SpecialFormsStatic {
  SValue* (*apply)            (SValue *sform, Evaluator eval, Env *env, SValue *args);
  bool    (*is_special_form)  (const char *token);
  SValue* (*from_string)      (const char *token);
  SValue* (*define)           (Evaluator eval, Env *env, SValue *args);
  SValue* (*lambda)           (Evaluator eval, Env *env, SValue *args);
};

extern const struct _SpecialFormsStatic SPECIAL_FORMS;
#endif
