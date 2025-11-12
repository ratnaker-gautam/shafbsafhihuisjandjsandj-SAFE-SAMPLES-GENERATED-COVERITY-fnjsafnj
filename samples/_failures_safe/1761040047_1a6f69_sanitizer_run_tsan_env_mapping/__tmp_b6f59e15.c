//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x) {
    return 3.14159 * x * x - 2.71828 * x + 1.61803;
}

double compute_integral(double a, double b, int steps) {
    if (steps <= 0 || a >= b) return 0.0;
    
    double sum = 0.0;
    double step_size = (b - a) / steps;
    
    for (int i = 0; i < steps; i++) {
        double x1 = a + i * step_size;
        double x2 = a + (i + 1) * step_size;
        sum += (compute_polynomial(x1) + compute_polynomial(x2)) * step_size / 2.0;
    }
    
    return sum;
}

int read_double(const char* prompt, double* value) {
    char buffer[MAX_INPUT_LEN];
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char* endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    
    return 1;
}

int read_int(const char* prompt, int* value) {
    char buffer[MAX_INPUT_LEN];
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char* endptr;
    long temp = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || temp < 1 || temp > 1000000) {
        return 0;
    }
    
    *value = (int)temp;
    return 1;
}

int main(void) {
    double a, b;
    int steps;
    
    printf("Numerical Integration Calculator\n");
    printf("Computing integral of f(x) = πx² - ex + φ\n\n");
    
    if (!read_double("Enter lower bound (a): ", &a)) {
        printf("Error: Invalid input for lower bound\n");
        return 1;
    }
    
    if (!read_double("Enter upper bound (b): ", &b)) {
        printf("Error: Invalid input for upper bound\n");
        return 1;
    }
    
    if (a >= b) {
        printf("Error: Lower bound must be less than upper bound\n");
        return 1;
    }
    
    if (!read_int("Enter number of steps (1-1000000): ", &steps)) {
        printf("Error: Invalid input for steps\n");
        return 1;
    }
    
    double result = compute_integral(a, b, steps);
    
    printf("\nIntegration Results:\n");
    printf("Bounds: [%.6f, %.6f]\n", a, b);
    printf("Steps: %d\n", steps);
    printf("Result: %.10f\n", result);
    
    printf("\nSample function values:\n");
    printf("f(%.2f) = %.6f\n", a, compute_polynomial(a));
    printf("f(%.2f) = %.6f\n", (a + b) / 2.0, compute_polynomial((a + b) / 2.0));
    printf("f(%.2f) = %.6f\n", b, compute_polynomial(b));
    
    return 0;
}