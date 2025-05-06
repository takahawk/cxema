#ifndef CXEMA_EVALUATOR_H_
#define CXEMA_EVALUATOR_H_

typedef struct Env Env;
typedef struct SValue SValue;

typedef SValue* (*Evaluator) (Env *env, SValue *val); 

// evaluate single clause, if cons it is evaluated as function
extern const Evaluator EVAL;

// evaluate multiple clauses, expected cons list and each
// element evaluated independently
// last value is returned
extern const Evaluator EVAL_ALL;

// evaluate multiple clauses, but last one, returning
// it unevaluated
extern const Evaluator EVAL_ALL_BUT_ONE;
#endif // !CXEMA_EVALUATOR_H_
