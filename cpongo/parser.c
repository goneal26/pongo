//
// Created by Tre on 6/17/24.
//

#include "parser.h"
#include "common.h"
#include "scanner.h"

typedef struct {
    Token current;
    Token previous;
} Parser;

Parser parser;

