//
// Created by Tre on 6/17/24.
//

#ifndef CPONGO_PARSER_H
#define CPONGO_PARSER_H

#include "scanner.h"

int accept(TokenType s);
int expect(TokenType s);
void primary();

#endif //CPONGO_PARSER_H
