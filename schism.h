#ifndef SCHISM_H_
#define SCHISM_H_

typedef struct Schism Schism;

struct Schism {
	SValue* (*interpret) (Schism *self, char *expression);
};

#endif