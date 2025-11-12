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
        } else if (isinf(result)) {
            printf("  f(%.2f) = infinity\n", x);
        } else {
            printf("  f(%.2f) = %.4f\n", x, result);
        }
    }
    printf("\n");
}

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "square_root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double start = 1.0;
    double end = 5.0;
    int steps = 8;
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, steps);
    }
    
    double test_values[] = {-4.0, 0.0, 2.0, 10.0};
    int num_tests = sizeof(test_values) / sizeof(test_values[0]);
    
    printf("Testing all functions with specific values:\n");
    for (int i = 0; i < num_tests; i++) {
        double x = test_values[i];
        printf("x = %.2f:\n", x);
        
        for (int j = 0; j < num_functions; j++) {
            double result = functions[j](x);
            
            if (isnan(result)) {
                printf("  %s: undefined\n", names[j]);
            } else if (isinf(result)) {
                printf("  %s: infinity\n", names[j]);
            } else {
                printf("  %s: %.4f\n", names[j], result);
            }
        }
        printf("\n");
    }
    
    return 0;
}