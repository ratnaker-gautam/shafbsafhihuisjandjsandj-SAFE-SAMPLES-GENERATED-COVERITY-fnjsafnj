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

void apply_function(numeric_func f, const char* name, double start, double end, int steps) {
    if (steps <= 0 || start >= end || f == NULL) {
        return;
    }
    
    double step_size = (end - start) / steps;
    if (step_size <= 0) {
        return;
    }
    
    printf("Applying %s from %.2f to %.2f in %d steps:\n", name, start, end, steps);
    
    for (int i = 0; i <= steps; i++) {
        double x = start + (i * step_size);
        double result = f(x);
        
        if (isnan(result)) {
            printf("  f(%.2f) = undefined\n", x);
        } else {
            printf("  f(%.2f) = %.4f\n", x, result);
        }
    }
    printf("\n");
}

int main(void) {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    const int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double start = 0.5;
    double end = 3.0;
    int steps = 10;
    
    if (start >= end || steps <= 0) {
        fprintf(stderr, "Invalid range or steps\n");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, steps);
    }
    
    printf("Demonstrating function pointer array usage:\n");
    double test_value = 2.5;
    for (int i = 0; i < num_functions; i++) {
        double result = functions[i](test_value);
        if (isnan(result)) {
            printf("%s(%.1f) = undefined\n", names[i], test_value);
        } else {
            printf("%s(%.1f) = %.4f\n", names[i], test_value, result);
        }
    }
    
    return EXIT_SUCCESS;
}