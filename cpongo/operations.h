//
// Created by Tre on 4/14/2024.
//

#ifndef CPONGO_OPERATIONS_H
#define CPONGO_OPERATIONS_H

// two argument arithmetic operations
short add(short a, short b);
short sub(short a, short b);
short mult(short a, short b);
short div(short a, short b);
short mod(short a, short b);

// one argument
short neg(short a); // negation
short not(short a); // bitwise not

// two argument bitwise/comparison operations
short equals(short a, short b);
short not_equals(short a, short b);
short gt(short a, short b);
short lt(short a, short b);
short gt_eq(short a, short b);
short lt_eq(short a, short b);
short and(short a, short b);
short or(short a, short b);

#endif //CPONGO_OPERATIONS_H
