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

void apply_function(numeric_func func, const char* name, double start, double end, int steps) {
    if (steps <= 0 || start > end || func == NULL) {
        return;
    }
    
    double step_size = (end - start) / steps;
    for (int i = 0; i <= steps; i++) {
        double x = start + i * step_size;
        double result = func(x);
        
        if (isnan(result)) {
            printf("%s(%.3f) = undefined\n", name, x);
        } else {
            printf("%s(%.3f) = %.3f\n", name, x, result);
        }
    }
    printf("\n");
}

int main() {
    double start_val, end_val;
    int num_steps;
    
    printf("Enter start value: ");
    if (scanf("%lf", &start_val) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter end value: ");
    if (scanf("%lf", &end_val) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter number of steps: ");
    if (scanf("%d", &num_steps) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_steps <= 0) {
        printf("Number of steps must be positive\n");
        return 1;
    }
    
    if (start_val > end_val) {
        printf("Start value must be less than or equal to end value\n");
        return 1;
    }
    
    numeric_func functions[] = {square, cube, reciprocal, square_root};
    const char* names[] = {"square", "cube", "reciprocal", "sqrt"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start_val, end_val, num_steps);
    }
    
    return 0;
}