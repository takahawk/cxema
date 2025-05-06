#ifndef CXEMA_SPECIAL_FORMS_H_
#define CXEMA_SPECIAL_FORMS_H_
#include <stdbool.h>

typedef struct Env Env;
typedef struct SValue SValue;
typedef struct Cxema Cxema;

typedef enum SpecialForm SpecialForm;
enum SpecialForm {
  SPECIAL_FORM_DEFINE,
  SPECIAL_FORM_LAMBDA,
  SPECIAL_FORM_COND,
  SPECIAL_FORM_IF,
  SPECIAL_FORM_AND,
  SPECIAL_FORM_OR,
};

struct _SpecialFormsStatic {
  // warning: apply can possibly return not completely evaluated results
  //          (for sake of possible tail call optimizations)
  SValue* (*apply)            (SValue *sform, Env *env, SValue *args);
  bool    (*is_special_form)  (const char *token);
  SValue* (*from_string)      (const char *token);

  SValue* (*define)           (Env *env, SValue *args);
  SValue* (*lambda)           (Env *env, SValue *args);
  SValue* (*cond)             (Env *env, SValue *args);
  SValue* (*_if)              (Env *env, SValue *args);
  SValue* (*and)              (Env *env, SValue *args);
  SValue* (*or)               (Env *env, SValue *args);
};

extern const struct _SpecialFormsStatic SPECIAL_FORMS;
#endif
