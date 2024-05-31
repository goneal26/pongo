//
// Created by Tre on 5/31/24.
//

#ifndef CPONGO_SCANNER_H
#define CPONGO_SCANNER_H

#include "token.h"

void scan_file(const char *path, Token tokens[]);
void scan_tokens(char *source, Token tokens[]);

#endif //CPONGO_SCANNER_H
