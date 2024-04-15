//
// Created by Tre on 4/14/2024.
//

#include "operations.h"
#include "errors.h"
#include <limits.h>

short add(short a, short b) {
    int actual = (int)(a + b);
    if (actual > SHRT_MAX || actual < SHRT_MIN) {
        warning("Number overflow.");
    }
    return (short)(a + b);
}

short sub(short a, short b) {
    int actual = (int)(a - b);
    if (actual > SHRT_MAX || actual < SHRT_MIN) {
        warning("Number overflow.");
    }
    return (short)(a - b);
}

short mult(short a, short b) {
    long long actual = (long long)(a * b);
    if (actual > SHRT_MAX || actual < SHRT_MIN) {
        warning("Number overflow.");
    }
    return (short)(a * b);
}

short div(short a, short b) {
    if (b == 0) {
        error_w_code("Divide by 0.", 133);
    }
    return (short)(a / b);
}

short mod(short a, short b) {
    return (short)(a % b);
}

short neg(short a) {
    int actual = (int)(-a);
    if (actual > SHRT_MAX || actual < SHRT_MIN) {
        warning("Number overflow.");
    }
    return (short)(-a);
}

short equals(short a, short b) {
    if (a == b) {
        return -1;
    } else {
        return 0;
    }
}

short not_equals(short a, short b) {
    if (a != b) {
        return -1;
    } else {
        return 0;
    }
}

short gt(short a, short b) {
    if (a > b) {
        return -1;
    } else {
        return 0;
    }
}

short lt(short a, short b) {
    if (a < b) {
        return -1;
    } else {
        return 0;
    }
}

short gt_eq(short a, short b) {
    if (a >= b) {
        return -1;
    } else {
        return 0;
    }
}

short lt_eq(short a, short b) {
    if (a <= b) {
        return -1;
    } else {
        return 0;
    }
}

short not(short a) {
    return (short)(~a);
}

short and(short a, short b) {
    return (short)(a & b);
}

short or(short a, short b) {
    return (short)(a | b);
}