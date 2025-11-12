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
    if (steps <= 0 || start >= end || func == NULL) {
        return;
    }
    
    double step_size = (end - start) / steps;
    if (step_size <= 0) {
        return;
    }
    
    printf("%s function results:\n", name);
    printf("x\t\tf(x)\n");
    printf("-------------------\n");
    
    for (int i = 0; i <= steps; i++) {
        double x = start + i * step_size;
        double result = func(x);
        
        if (isnan(result)) {
            printf("%.3f\t\tundefined\n", x);
        } else if (isinf(result)) {
            printf("%.3f\t\tinfinity\n", x);
        } else {
            printf("%.3f\t\t%.6f\n", x, result);
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
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter end value: ");
    if (scanf("%lf", &end) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (start >= end) {
        printf("Start must be less than end\n");
        return 1;
    }
    
    printf("Enter number of steps: ");
    if (scanf("%d", &steps) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (steps <= 0 || steps > 10000) {
        printf("Steps must be between 1 and 10000\n");
        return 1;
    }
    
    printf("\nNumeric Function Analysis\n");
    printf("Range: [%.3f, %.3f], Steps: %d\n\n", start, end, steps);
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, steps);
    }
    
    return 0;
}