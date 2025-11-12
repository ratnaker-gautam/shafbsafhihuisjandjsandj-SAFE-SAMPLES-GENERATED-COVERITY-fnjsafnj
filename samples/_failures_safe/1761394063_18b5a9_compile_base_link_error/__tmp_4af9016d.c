//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef double (*numeric_func)(double);

double linear(double x) {
    return 2.0 * x + 1.0;
}

double quadratic(double x) {
    return x * x - 3.0 * x + 2.0;
}

double exponential(double x) {
    return exp(x / 2.0);
}

double trigonometric(double x) {
    return sin(x) + cos(x);
}

void compute_range(numeric_func func, double start, double end, int steps) {
    if (steps <= 0 || start >= end) {
        printf("Invalid range parameters\n");
        return;
    }
    
    double step_size = (end - start) / steps;
    for (int i = 0; i <= steps; i++) {
        double x = start + i * step_size;
        double result = func(x);
        printf("f(%.3f) = %.6f\n", x, result);
    }
}

int main() {
    numeric_func functions[] = {linear, quadratic, exponential, trigonometric};
    char* names[] = {"Linear", "Quadratic", "Exponential", "Trigonometric"};
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
    
    for (int i = 0; i < num_functions; i++) {
        printf("\n%s function:\n", names[i]);
        compute_range(functions[i], start, end, steps);
    }
    
    return 0;
}