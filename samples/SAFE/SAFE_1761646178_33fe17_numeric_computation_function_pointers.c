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
    if (fabs(x) < 1e-15) {
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
    if (steps <= 0 || start > end || f == NULL) {
        printf("Invalid parameters for %s\n", name);
        return;
    }
    
    double step_size = (end - start) / steps;
    if (isnan(step_size) || isinf(step_size)) {
        printf("Invalid step size for %s\n", name);
        return;
    }
    
    printf("%s function results:\n", name);
    for (int i = 0; i <= steps; i++) {
        double x = start + i * step_size;
        if (isnan(x) || isinf(x)) {
            printf("x = invalid, f(x) = invalid\n");
            continue;
        }
        
        double result = f(x);
        if (isnan(result)) {
            printf("x = %.3f, f(x) = undefined\n", x);
        } else if (isinf(result)) {
            printf("x = %.3f, f(x) = infinity\n", x);
        } else {
            printf("x = %.3f, f(x) = %.3f\n", x, result);
        }
    }
    printf("\n");
}

int main() {
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"Square", "Cube", "Reciprocal", "Square Root"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    double start, end;
    int steps;
    
    printf("Enter start value: ");
    if (scanf("%lf", &start) != 1) {
        printf("Invalid start value\n");
        return 1;
    }
    
    printf("Enter end value: ");
    if (scanf("%lf", &end) != 1) {
        printf("Invalid end value\n");
        return 1;
    }
    
    printf("Enter number of steps: ");
    if (scanf("%d", &steps) != 1) {
        printf("Invalid number of steps\n");
        return 1;
    }
    
    if (steps < 1 || steps > 10000) {
        printf("Number of steps must be between 1 and 10000\n");
        return 1;
    }
    
    if (isnan(start) || isnan(end) || isinf(start) || isinf(end)) {
        printf("Start and end values must be finite numbers\n");
        return 1;
    }
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, steps);
    }
    
    return 0;
}