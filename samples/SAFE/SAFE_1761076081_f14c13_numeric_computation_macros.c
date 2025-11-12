//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0

#define VALIDATE_INPUT(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))

double newton_sqrt(double n) {
    if (n < 0) return -1.0;
    if (n == 0.0) return 0.0;
    
    double x = n;
    double prev;
    int iter = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iter++;
    } while (ABS(x - prev) > EPSILON && iter < MAX_ITERATIONS);
    
    return x;
}

double compute_polynomial(double x, double a, double b, double c, double d) {
    return a * SQUARE(SQUARE(x)) + b * SQUARE(x) * x + c * SQUARE(x) + d;
}

int main() {
    double input, a, b, c, d;
    char buffer[256];
    
    printf("Enter a number between %.1f and %.1f: ", MIN_INPUT, MAX_INPUT);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(buffer, "%lf", &input) != 1) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(input)) {
        fprintf(stderr, "Input out of valid range\n");
        return 1;
    }
    
    printf("Enter coefficients a, b, c, d for polynomial ax^4 + bx^3 + cx^2 + d: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading coefficients\n");
        return 1;
    }
    
    if (sscanf(buffer, "%lf %lf %lf %lf", &a, &b, &c, &d) != 4) {
        fprintf(stderr, "Invalid coefficient format\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b) || !VALIDATE_INPUT(c) || !VALIDATE_INPUT(d)) {
        fprintf(stderr, "Coefficient out of valid range\n");
        return 1;
    }
    
    double sqrt_result = newton_sqrt(input);
    if (sqrt_result < 0) {
        fprintf(stderr, "Cannot compute square root of negative number\n");
        return 1;
    }
    
    double poly_result = compute_polynomial(input, a, b, c, d);
    
    printf("Square root of %.6f: %.6f\n", input, sqrt_result);
    printf("Polynomial result: %.6f\n", poly_result);
    printf("Combined result: %.6f\n", sqrt_result + poly_result);
    
    return 0;
}