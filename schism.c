#include "schism.h"

static SValue* interpret(char *code) {
	// TODO: implement
	return form_svalue_num(0);

}

const Schism SCHISM_PROTOTYPE = {
	.interpret = interpret
};

// TODO: custom allocator?
Schism *form_schism() {
	Schism *schism = malloc(sizeof(*schism));
	return schism;
}

