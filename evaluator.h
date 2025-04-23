#ifndef CXEMA_EVALUATOR_H_
#define CXEMA_EVALUATOR_H_

typedef struct Env Env;
typedef struct SValue SValue;

typedef SValue* (*Evaluator) (Env *env, SValue *val); 

extern const Evaluator EVAL;
#endif // !CXEMA_EVALUATOR_H_
