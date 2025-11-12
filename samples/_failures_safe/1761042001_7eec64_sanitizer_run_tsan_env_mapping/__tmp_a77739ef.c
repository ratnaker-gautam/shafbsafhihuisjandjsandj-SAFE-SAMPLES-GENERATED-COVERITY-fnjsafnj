//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

typedef double (*numeric_func)(double);

double square(double x) {
    return x * x;
}

double cube(double x) {
    return x * x * x;
}

double reciprocal(double x) {
    if (fabs(x) < 1e-12) {
        return NAN;
    }
    return 1.0 / x;
}

double square_root(double x) {
    if (x < 0) {
        return NAN;
    }
    return sqrt(x);
}

int validate_input(const char *input, double *result) {
    char *endptr;
    double val = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    if (val == HUGE_VAL || val == -HUGE_VAL) {
        return 0;
    }
    *result = val;
    return 1;
}

void apply_function(numeric_func func, double x, const char *name) {
    double result = func(x);
    if (isnan(result)) {
        printf("Function %s: undefined for input %g\n", name, x);
    } else {
        printf("Function %s(%g) = %g\n", name, x, result);
    }
}

int main(void) {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char *names[] = {"square", "cube", "reciprocal", "square_root"};
    const int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    char input[256];
    double x;
    
    printf("Enter a numeric value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (!validate_input(input, &x)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], x, names[i]);
    }
    
    return 0;
}