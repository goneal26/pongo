//
// Created by Tre on 5/2/2024.
//

#include "expression.h"
#include <ctype.h>
#include <string.h>
#include "common.h"
#include "messages.h"
#include <time.h>

typedef enum {
    NOOP,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    NEGATE,
    MOD,
    AND,
    OR,
    NOT,
    EQUALS,
    NOT_EQUALS,
    GREATER,
    LESS,
    GREATER_EQUALS,
    LESS_EQUALS,
} Operation;

/* Created this for debugging
char *opnames[] = {
        "NOOP",
        "ADD",
        "SUBTRACT",
        "MULTIPLY",
        "DIVIDE",
        "NEGATE",
        "MOD",
        "AND",
        "OR",
        "NOT",
        "EQUALS",
        "NOT_EQUALS",
        "GREATER",
        "LESS",
        "GREATER_EQUALS",
        "LESS_EQUALS",
};
*/

typedef struct Expression {
    Operation operator;
    short operand1;
    short operand2;
    bool is_unary;
} Expression;

short evalexpr(Expression e);
void new_expression(const char* source, Expression *e, const ShortTable* table);
bool isoper(int c);
void get_first_operand(const char* source, char *dest);
void get_second_operand(const char* source, char *dest);
short convert_identifier(const char* source, const ShortTable* table);
Operation get_operator(const char* source);

/*
 * Given an expression string, evaluate that expression and return a short
 */
short eval(const char* source, const ShortTable* table) {
    Expression e;
    new_expression(source, &e, table);
    // printf("Arg 1: %d\n", e.operand1);
    // printf("Arg 2: %d\n", e.operand2);
    // printf("Operation: %s\n", opnames[e.operator]);
    // printf("Is it unary? %d\n", e.is_unary);
    return evalexpr(e);
}

short evalexpr(Expression e) {
    if (e.is_unary) { // NEG, NOT, or NOOP
        switch (e.operator) {
            case NEGATE:
                return (short) -e.operand2;
            case NOT:
                return (short) ~e.operand2;
            case NOOP:
                return e.operand1;
        }
    } else {
        switch(e.operator) {
            case ADD:
                return (short) (e.operand1 + e.operand2);
            case SUBTRACT:
                return (short) (e.operand1 - e.operand2);
            case MULTIPLY:
                return (short) (e.operand1 * e.operand2);
            case DIVIDE:
                if (e.operand2 == 0) {
                    error("Divide by 0.");
                }
                return (short) (e.operand1 / e.operand2);
            case MOD:
                if (e.operand2 == 0) {
                    error("Modulo 0");
                }
                return (short) (e.operand1 % e.operand2);
            case AND:
                return (short) (e.operand1 & e.operand2);
            case OR:
                return (short) (e.operand1 | e.operand2);
            case EQUALS:
                if (e.operand1 == e.operand2) {
                    return (short) -1;
                } else {
                    return 0;
                }
            case NOT_EQUALS:
                if (e.operand1 != e.operand2) {
                    return (short) -1;
                } else {
                    return 0;
                }
            case GREATER:
                if (e.operand1 > e.operand2) {
                    return (short) -1;
                } else {
                    return 0;
                }
            case LESS:
                if (e.operand1 < e.operand2) {
                    return (short) -1;
                } else {
                    return 0;
                }
            case GREATER_EQUALS:
                if (e.operand1 >= e.operand2) {
                    return (short) -1;
                } else {
                    return 0;
                }
            case LESS_EQUALS:
                if (e.operand1 <= e.operand2) {
                    return (short) -1;
                } else {
                    return 0;
                }
        }
    }
}

/*
 * Converts a given string identifier/literal into a short for manipulation
 */
short convert_identifier(const char* source, const ShortTable* table) {
    // printf("Converting identifier \"%s\"\n", source);
    // assume non-empty string
    if (isalpha(source[0]) || source[0] == '_') {
        if (strcmp(source, "true") == 0) { // boolean macros
            return (signed short) 0xffff;
        } else if (strcmp(source, "false") == 0) {
            return (signed short) 0;
        } else if (strcmp(source, "rand") == 0) { // adding rand!
            srand(time(NULL));
            int random_value = rand() % (0xffff + 1); // random num between 0 and 0xffff inclusive
            return (signed short) random_value;
        } else {
            return get_value(source, table);
        }
    } else if (isdigit(source[0])) { // number literal
        if (source[1] != '\0' && ((char) tolower(source[1])) == 'x') { // hex literal
            return (short) strtol(source, NULL, 0);
        } else { // decimal literal
            return (short) strtol(source, NULL, 10);
        }
    } else {
        error("Invalid char.");
    }
}

/*
 * Places each token in an expression string (source) into an Expression struct (e)
 */
void new_expression(const char* source, Expression *e, const ShortTable* table) { // array with { operator, operand2, operand1 }
    int length = (int) strlen(source) + 1;
    char trimmed[length];
    char op1[length];
    char op2[length];
    Operation operator;
    trim(source, trimmed);
    // printf("Trimmed expression: \"%s\"\n", trimmed);

    operator = get_operator(trimmed);
    get_second_operand(trimmed, op2);
    get_first_operand(trimmed, op1);

    // printf("Got first arg: \"%s\"\n", op1);
    // printf("Got second arg: \"%s\"\n", op2);

    if (op1[0] == '\0') {
        e->is_unary = true;
        if (operator == SUBTRACT) {
            e->operator = NEGATE;
        } else {
            e->operator = operator;
        }
        e->operand1 = (short) 0;
        e->operand2 = convert_identifier(op2, table);
    } else if (op2[0] == '\0') {
        e->is_unary = true;
        e->operand1 = convert_identifier(op1, table);
        e->operand2 = (short) 0;
        e->operator = operator;
    } else {
        e->is_unary = false;
        e->operator = operator;
        e->operand1 = convert_identifier(op1, table);
        e->operand2 = convert_identifier(op2, table);
    }

}

/*
 * Remove leading and trailing whitespace, store in dest
 */
void trim(const char* source, char* dest) {
    // printf("Pre-trim: \"%s\"\n", source);
    if (*source == 0) {
        *dest = 0;
        return;
    }

    const char *end;

    // trim leading space
    while(isspace((unsigned char)*source)) source++;

    // all spaces?
    if (*source == 0) {
        *dest = 0;
        return;
    }

    // trim trailing space
    end = source + strlen(source) - 1;
    while(end > source && isspace(*end)) end--;

    while (source <= end) {
        *dest = *source;
        dest++;
        source++;
    }

    *dest = 0;
}

/*
 * Check if a provided char (int) is an "operator" (1 if true, 0 if false)
 */
bool isoper(int c) {
    if (c == '+' || c == '-' || c == '*' ||
        c == '/' || c == '%' || c == '&' ||
        c == '|' || c == '!' || c == '>' ||
        c == '<' || c == '=') {
        return true;
    } else {
        return false;
    }
}

/*
 * Writes the substring for the expression's operator into dest
 */
Operation get_operator(const char* source) {
    while (*source) {
        char c = *source;

        if (isoper(c) && *(source + 1)) { // c is an operator and there is a next char
            if ((c == '=' || c == '>' || c == '<' || c == '!') && *(source + 1) == '=') { // c is a 2-character comparison operator
                switch (c) {
                    case '>':
                        return GREATER_EQUALS;
                    case '<':
                        return LESS_EQUALS;
                    case '!':
                        return NOT_EQUALS;
                    case '=':
                        return EQUALS;
                } // shouldn't need a default case, this is all the possible cases here
            } else { // c is a single-character operator
                // printf("Found opchar: %c\n", c);
                switch (c) {
                    case '+':
                        return ADD;
                    case '-':
                        return SUBTRACT;
                    case '*':
                        return MULTIPLY;
                    case '/':
                        return DIVIDE;
                    case '%':
                        return MOD;
                    case '&':
                        return AND;
                    case '|':
                        return OR;
                    case '=':
                        error("Invalid assignment.");
                        break;
                    case '>':
                        return GREATER;
                    case '<':
                        return LESS;
                    case '!':
                        // printf("Found NOT\n");
                        return NOT;
                    default:
                        continue;
                }
            }
        }

        source++;
    }

    // printf("Am I getting to here?\n");
    // if no operators found, return noop
    return NOOP;
}

/*
 * Writes substring for expression's first operand into dest
 * (input is a TRIMMED string)
 */
void get_first_operand(const char* source, char *dest) {
    // assume source has already been trimmed
    while (*source) {
        if (isalnum((unsigned char) *source) || *source == '_') {
            *dest = *source;
            dest++;
        } else {
            break;
        }

        source++;
    }

    *dest = 0;
}

/*
 * Writes substring for expression's second operand into dest
 * (input is a TRIMMED string)
 */
void get_second_operand(const char* source, char *dest) {
    // assume source has already been trimmed
    const char* end = source + strlen(source) - 1;
    while (end > source) {
        if (isalnum(*end) || *end == '_') {
            end--;
        } else {
            break;
        }
    } // end ptr now at first character of second operand

    // is there just no argument?
    if (source == end && !isoper(*end)) {
        *dest = 0;
        return;
    }


    end++; // correcting how end pointer goes 1 spot too far

    while (*end) {
        *dest = *end;
        dest++;
        end++;
    }

    *dest = 0;
}
