#ifndef CXEMA_SPECIAL_FORMS_H_
#define CXEMA_SPECIAL_FORMS_H_
#include <stdbool.h>

#include "codex/mem/rc.h"

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
  SValue* (*apply)            (SValue *sform, Rc* /*Env**/ env, SValue *args);
  bool    (*is_special_form)  (const char *token);
  SValue* (*from_string)      (const char *token);

  SValue* (*define)           (Rc* /*Env**/ env, SValue *args);
  SValue* (*lambda)           (Rc* /*Env**/ env, SValue *args);
  SValue* (*cond)             (Rc* /*Env**/ env, SValue *args);
  SValue* (*_if)              (Rc* /*Env**/ env, SValue *args);
  SValue* (*and)              (Rc* /*Env**/ env, SValue *args);
  SValue* (*or)               (Rc* /*Env**/ env, SValue *args);
};

extern const struct _SpecialFormsStatic SPECIAL_FORMS;
#endif
