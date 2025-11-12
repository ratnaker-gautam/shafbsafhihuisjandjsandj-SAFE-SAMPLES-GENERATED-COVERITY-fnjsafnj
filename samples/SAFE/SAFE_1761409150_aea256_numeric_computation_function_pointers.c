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
    if (steps <= 0 || start > end) {
        printf("Invalid range or steps\n");
        return;
    }
    
    double step_size = (end - start) / steps;
    printf("%s function results:\n", name);
    printf("x\t\tf(x)\n");
    printf("-------------------\n");
    
    for (int i = 0; i <= steps; i++) {
        double x = start + i * step_size;
        double result = f(x);
        
        if (isnan(result)) {
            printf("%.3f\t\tundefined\n", x);
        } else {
            printf("%.3f\t\t%.3f\n", x, result);
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
    
    printf("Enter number of steps: ");
    if (scanf("%d", &steps) != 1 || steps <= 0) {
        printf("Invalid number of steps\n");
        return 1;
    }
    
    if (start > end) {
        printf("Start value must be less than or equal to end value\n");
        return 1;
    }
    
    for (int i = 0; i < num_functions; i++) {
        apply_function(functions[i], names[i], start, end, steps);
    }
    
    return 0;
}