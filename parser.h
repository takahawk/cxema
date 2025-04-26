#ifndef CXEMA_PARSER_H_
#define CXEMA_PARSER_H_

typedef struct SValue SValue;

typedef SValue* (*Parser) (const char *code); 

extern const Parser PARSE;

#endif // !CXEMA_PARSER_H_

