//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
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
    if (fabs(x) < 1e-10) {
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
    if (input == NULL) {
        return 0;
    }
    char *endptr;
    double val = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        return 0;
    }
    if (val == HUGE_VAL || val == -HUGE_VAL) {
        return 0;
    }
    *result = val;
    return 1;
}

void apply_function(numeric_func func, double x, const char *func_name) {
    double result = func(x);
    if (isnan(result)) {
        printf("%s(%.2f) = undefined\n", func_name, x);
    } else {
        printf("%s(%.2f) = %.4f\n", func_name, x, result);
    }
}

int main(void) {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char *names[] = {"square", "cube", "reciprocal", "square_root"};
    const int num_funcs = sizeof(functions) / sizeof(functions[0]);
    
    char input[256];
    printf("Enter a numeric value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    double value;
    if (!validate_input(input, &value)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Applying numeric functions to %.2f:\n", value);
    for (int i = 0; i < num_funcs; i++) {
        apply_function(functions[i], value, names[i]);
    }
    
    return 0;
}