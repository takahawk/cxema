#include "cxema.h"
#include "svalue.h"

#include "asserts.h"

int main() {
	Cxema *cx = CXEMA.form();

  // 1.1.1 Expressions
  assert_interprets_as(cx, "486", SVAL_TYPE_INT, "486", __FILE__, __LINE__);
  assert_interprets_as(cx, "(+ 137 349)", SVAL_TYPE_INT, "486", __FILE__, __LINE__);
  assert_interprets_as(cx, "(- 1000 334)", SVAL_TYPE_INT, "666", __FILE__, __LINE__);
  assert_interprets_as(cx, "(* 5 99)", SVAL_TYPE_INT, "495", __FILE__, __LINE__);
  assert_interprets_as(cx, "(/ 10 5)", SVAL_TYPE_INT, "2", __FILE__, __LINE__);
  assert_interprets_as(cx, "(+ 2.7 10)", SVAL_TYPE_FLOAT, "12.7", __FILE__, __LINE__);
  assert_interprets_as(cx, "(+ 21 35 12 7)", SVAL_TYPE_INT, "75", __FILE__, __LINE__);
  assert_interprets_as(cx, "(* 25 4 12)", SVAL_TYPE_INT, "1200", __FILE__, __LINE__);
  assert_interprets_as(cx, "(+ (* 3 5) (- 10 6))", SVAL_TYPE_INT, "19", __FILE__, __LINE__);
  assert_interprets_as(cx, "(+ (* 3 (+ (* 2 4) (+ 3 5))) (+ (- 10 7) 6))",
                       SVAL_TYPE_INT, "57", __FILE__, __LINE__);

  // 1.1.2 Naming and the Environment
  assert_interprets_as(cx, "(define size 2)", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "size", SVAL_TYPE_INT, "2", __FILE__, __LINE__);
  assert_interprets_as(cx, "(* size 5)", SVAL_TYPE_INT, "10", __FILE__, __LINE__);

  assert_interprets_as(cx, "(define pi 3.14159)", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(define radius 10)", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(* pi (* radius radius))",
                       SVAL_TYPE_FLOAT, "314.159", __FILE__, __LINE__);

  cx->interpret(cx, "(define circumference)");

  // 1.1.3 Evaluating combinations
  assert_interprets_as(cx, "(* (+ 2 (* 4 6)) (+ 3 5 7))", SVAL_TYPE_INT, "390", __FILE__, __LINE__);

  // 1.1.4 Compound procedures
  assert_interprets_as(cx, "(define (square x) (* x x))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(square 21)", SVAL_TYPE_INT, "441", __FILE__, __LINE__);
  assert_interprets_as(cx, "(square (+ 2 5))", SVAL_TYPE_INT, "49", __FILE__, __LINE__);
  assert_interprets_as(cx, "(square (square 3))", SVAL_TYPE_INT, "81", __FILE__, __LINE__);
  assert_interprets_as(cx, "(define (sum-of-squares x y) (+ (square x) (square y)))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(define (f a) (sum-of-squares (+ a 1) (* a 2)))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(f 5)", SVAL_TYPE_INT, "136", __FILE__, __LINE__);

  // 1.1.6 Conditional Expressions and Predicates
  assert_interprets_as(cx, "(define (abs x)\n" 
                           "  (cond ((> x 0) x)\n"
                           "        ((= x 0) 0)\n"
                           "        ((< x 0) (- x))))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);

  assert_interprets_as(cx, "(abs 1337)", SVAL_TYPE_INT, "1337", __FILE__, __LINE__);
  assert_interprets_as(cx, "(abs (- 1337))", SVAL_TYPE_INT, "1337", __FILE__, __LINE__);
  assert_interprets_as(cx, "(define (abs x)\n" 
                           "  (cond ((< x 0) (- x))\n"
                           "        (else x)))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(abs 1337)", SVAL_TYPE_INT, "1337", __FILE__, __LINE__);
  assert_interprets_as(cx, "(abs (- 1337))", SVAL_TYPE_INT, "1337", __FILE__, __LINE__);

  assert_interprets_as(cx, "(define (abs x)\n" 
                           "  (if (< x 0)\n"
                           "      (- x)\n"
                           "      x))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(abs 1337)", SVAL_TYPE_INT, "1337", __FILE__, __LINE__);
  assert_interprets_as(cx, "(abs (- 1337))", SVAL_TYPE_INT, "1337", __FILE__, __LINE__);

  assert_interprets_as(cx, "(define x 7)", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(and (> x 5) (< x 10))", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);

  assert_interprets_as(cx, "(define (>= x y) (or (> x y) (= x y)))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 1337 1337)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 0111 1337)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 1337 0111)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);

  assert_interprets_as(cx, "(define (>= x y) (not (< x y)))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 1337 1337)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 0111 1337)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 1337 0111)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
	cx->release(&cx);
	return 0;
}
