//
// Created by Tre on 5/31/24.
//

#ifndef CPONGO_INTERPRETER_H
#define CPONGO_INTERPRETER_H

#include "scanner.h"

void print_token(const Token *t);
void interpret(const char *source);

#endif //CPONGO_INTERPRETER_H
