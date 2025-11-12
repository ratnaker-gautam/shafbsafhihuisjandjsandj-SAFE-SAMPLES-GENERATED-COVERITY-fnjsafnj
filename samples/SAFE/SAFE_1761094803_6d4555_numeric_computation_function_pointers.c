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
    if (x < 0.0) {
        return NAN;
    }
    return sqrt(x);
}

void apply_function(numeric_func func, const char* name, double start, double end, double step) {
    if (step <= 0.0 || start > end || func == NULL) {
        printf("Invalid parameters for %s\n", name);
        return;
    }
    
    printf("%s:\n", name);
    for (double x = start; x <= end; x += step) {
        double result = func(x);
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
    const int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double start, end, step;
    
    printf("Enter start value: ");
    if (scanf("%lf", &start) != 1) {
        printf("Invalid input for start value\n");
        return 1;
    }
    
    printf("Enter end value: ");
    if (scanf("%lf", &end) != 1) {
        printf("Invalid input for end value\n");
        return 1;
    }
    
    printf("Enter step size: ");
    if (scanf("%lf", &step) != 1) {
        printf("Invalid input for step size\n");
        return 1;
    }
    
    if (step <= 0.0) {
        printf("Step size must be positive\n");
        return 1;
    }
    
    if (start > end) {
        printf("Start value must be less than or equal to end value\n");
        return 1;
    }
    
    if (end - start > 1000.0 * step) {
        printf("Too many iterations requested\n");
        return 1;
    }
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, step);
    }
    
    return 0;
}