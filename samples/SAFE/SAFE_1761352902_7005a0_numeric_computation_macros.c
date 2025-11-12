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
    int iter = 0;
    
    do {
        prev = x;
        if (ABS(prev) < EPSILON) break;
        x = 0.5 * (prev + n / prev);
        iter++;
    } while (ABS(x - prev) > EPSILON && iter < MAX_ITERATIONS);
    
    return x;
}

double compute_quadratic(double a, double b, double c) {
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b) || !VALIDATE_INPUT(c)) {
        return NAN;
    }
    
    if (ABS(a) < EPSILON) {
        if (ABS(b) < EPSILON) return NAN;
        return -c / b;
    }
    
    double discriminant = SQUARE(b) - 4.0 * a * c;
    
    if (discriminant < 0) return NAN;
    
    double sqrt_disc = newton_sqrt(discriminant);
    if (sqrt_disc < 0) return NAN;
    
    double root1 = (-b + sqrt_disc) / (2.0 * a);
    double root2 = (-b - sqrt_disc) / (2.0 * a);
    
    return (ABS(root1) > ABS(root2)) ? root1 : root2;
}

int main() {
    double a, b, c;
    
    printf("Enter coefficients a, b, c for quadratic equation ax^2 + bx + c = 0:\n");
    
    if (scanf("%lf %lf %lf", &a, &b, &c) != 3) {
        printf("Error: Invalid input format.\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b) || !VALIDATE_INPUT(c)) {
        printf("Error: Coefficients must be between %.1f and %.1f.\n", MIN_INPUT, MAX_INPUT);
        return 1;
    }
    
    double result = compute_quadratic(a, b, c);
    
    if (isnan(result)) {
        printf("No real solution exists for the given coefficients.\n");
    } else {
        printf("Selected root: %.6f\n", result);
    }
    
    return 0;
}