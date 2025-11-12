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
    if (x < 0.0) {
        return NAN;
    }
    return sqrt(x);
}

void apply_function(numeric_func func, const char* name, double start, double end, int steps) {
    if (steps <= 0 || start >= end || func == NULL) {
        return;
    }
    
    double step_size = (end - start) / steps;
    if (step_size <= 0.0) {
        return;
    }
    
    printf("%s:\n", name);
    for (int i = 0; i <= steps; i++) {
        double x = start + (i * step_size);
        double result = func(x);
        
        if (isnan(result)) {
            printf("  f(%.3f) = undefined\n", x);
        } else if (isinf(result)) {
            printf("  f(%.3f) = infinity\n", x);
        } else {
            printf("  f(%.3f) = %.3f\n", x, result);
        }
    }
    printf("\n");
}

int main(void) {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"Square", "Cube", "Reciprocal", "Square Root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double start = 0.0;
    double end = 5.0;
    int steps = 10;
    
    if (start >= end || steps <= 0) {
        fprintf(stderr, "Invalid range or steps\n");
        return EXIT_FAILURE;
    }
    
    printf("Numeric function evaluation from %.1f to %.1f in %d steps:\n\n", start, end, steps);
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, steps);
    }
    
    return EXIT_SUCCESS;
}