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
    if (steps <= 0 || start > end || func == NULL) {
        return;
    }
    
    double step_size = (end - start) / steps;
    if (isnan(step_size) || isinf(step_size)) {
        return;
    }
    
    printf("%s:\n", name);
    for (int i = 0; i <= steps; i++) {
        double x = start + i * step_size;
        if (isnan(x) || isinf(x)) {
            continue;
        }
        
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

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"Square", "Cube", "Reciprocal", "Square Root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double start = -2.0;
    double end = 2.0;
    int steps = 10;
    
    if (steps <= 0 || start > end) {
        fprintf(stderr, "Invalid range parameters\n");
        return 1;
    }
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, steps);
    }
    
    return 0;
}