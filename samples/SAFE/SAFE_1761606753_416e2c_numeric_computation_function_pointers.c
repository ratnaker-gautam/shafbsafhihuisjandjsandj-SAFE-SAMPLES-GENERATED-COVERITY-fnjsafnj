//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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

void apply_function(numeric_func func, const char* name, double start, double end, double step) {
    if (step <= 0.0 || start > end || !func) {
        printf("Invalid parameters.\n");
        return;
    }
    
    printf("%s function from %.2f to %.2f (step %.2f):\n", name, start, end, step);
    
    double x = start;
    while (x <= end) {
        double result = func(x);
        if (isnan(result)) {
            printf("  f(%.2f) = undefined\n", x);
        } else {
            printf("  f(%.2f) = %.4f\n", x, result);
        }
        
        if (x > DBL_MAX - step) {
            break;
        }
        x += step;
    }
    printf("\n");
}

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"Square", "Cube", "Reciprocal", "Square Root"};
    const int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    printf("Numeric Function Computation Demo\n");
    printf("=================================\n\n");
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], 1.0, 5.0, 1.0);
    }
    
    printf("Additional test with different range:\n");
    apply_function(reciprocal, "Reciprocal", -2.0, 2.0, 0.5);
    
    return 0;
}