//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void apply_function(numeric_func f, const char* name, double start, double end, int steps) {
    if (steps <= 0 || start >= end || f == NULL) {
        return;
    }
    
    double step_size = (end - start) / steps;
    printf("%s function results:\n", name);
    
    for (int i = 0; i <= steps; i++) {
        double x = start + i * step_size;
        double result = f(x);
        
        if (isnan(result)) {
            printf("  f(%.2f) = undefined\n", x);
        } else {
            printf("  f(%.2f) = %.4f\n", x, result);
        }
    }
    printf("\n");
}

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"Square", "Cube", "Reciprocal", "Square Root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double start = 0.5;
    double end = 3.0;
    int steps = 10;
    
    printf("Numeric function evaluation from %.1f to %.1f in %d steps:\n\n", start, end, steps);
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, steps);
    }
    
    printf("Computing function compositions:\n");
    for (int i = 0; i < num_functions; i++) {
        for (int j = 0; j < num_functions; j++) {
            if (i != j) {
                double test_val = 2.0;
                double first_result = functions[i](test_val);
                if (!isnan(first_result)) {
                    double second_result = functions[j](first_result);
                    if (!isnan(second_result)) {
                        printf("%s(%s(%.1f)) = %.4f\n", names[j], names[i], test_val, second_result);
                    }
                }
            }
        }
    }
    
    return 0;
}