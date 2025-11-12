//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
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
    int iterations = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iterations++;
    } while (ABS(x - prev) > EPSILON && iterations < MAX_ITERATIONS);
    
    return x;
}

double compute_quadratic(double a, double b, double c) {
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b) || !VALIDATE_INPUT(c)) {
        return NAN;
    }
    
    double discriminant = SQUARE(b) - 4 * a * c;
    
    if (discriminant < 0) {
        return NAN;
    }
    
    double root1 = (-b + newton_sqrt(discriminant)) / (2 * a);
    double root2 = (-b - newton_sqrt(discriminant)) / (2 * a);
    
    return (root1 + root2) / 2.0;
}

int main() {
    double a, b, c;
    
    printf("Enter coefficients a, b, c for quadratic equation ax^2 + bx + c = 0:\n");
    
    if (scanf("%lf %lf %lf", &a, &b, &c) != 3) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b) || !VALIDATE_INPUT(c)) {
        printf("Error: Input values out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return 1;
    }
    
    if (ABS(a) < EPSILON) {
        printf("Error: Coefficient a cannot be zero\n");
        return 1;
    }
    
    double result = compute_quadratic(a, b, c);
    
    if (isnan(result)) {
        printf("No real roots exist for the given coefficients\n");
    } else {
        printf("Average of real roots: %.10f\n", result);
    }
    
    return 0;
}