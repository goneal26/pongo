//
// Created by Tre on 6/25/24.
//

#include "parser.h"
#include "common.h"

typedef enum {
    AST_PRIMARY,
    AST_UNARY,
    AST_FACTOR,
    AST_TERM,
    AST_COMPARISON,
    AST_EQUALITY,
    AST_CONJUNCTION,
    AST_DISJUNCTION
} NodeType;

typedef enum {
    OP_POS,
    OP_NEG,
    OP_NOT,
    OP_DIV,
    OP_MULT,
    OP_MOD,
    OP_ADD,
    OP_SUB,
    OP_GT,
    OP_LT,
    OP_GE,
    OP_LE,
    OP_EQ,
    OP_NE,
    OP_AND,
    OP_OR
} Operator;

typedef struct ASTNode {
    Token value;
    NodeType type;
    Operator op;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

typedef struct {
    Token *tokens; // a cvector of tokens
} Parser;

Parser parser;

void init_parser(Token *tokens) {
    parser.tokens = tokens;
}