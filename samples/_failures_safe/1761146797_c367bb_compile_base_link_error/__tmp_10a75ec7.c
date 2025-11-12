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
#define CUBE(x) ((x) * (x) * (x))

double compute_polynomial(double x) {
    return 3.0 * SQUARE(x) - 2.0 * x + 5.0;
}

double compute_exponential(double x) {
    return exp(x / 2.0);
}

double compute_trigonometric(double x) {
    return sin(x) + cos(2.0 * x);
}

double find_root_bisection(double (*f)(double), double a, double b) {
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b)) {
        return NAN;
    }
    
    if (f(a) * f(b) >= 0.0) {
        return NAN;
    }
    
    double c = a;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        c = (a + b) / 2.0;
        
        if (fabs(f(c)) < EPSILON || fabs(b - a) < EPSILON) {
            return c;
        }
        
        if (f(c) * f(a) < 0.0) {
            b = c;
        } else {
            a = c;
        }
    }
    
    return NAN;
}

int main() {
    double a, b;
    
    printf("Enter interval start: ");
    if (scanf("%lf", &a) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter interval end: ");
    if (scanf("%lf", &b) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b)) {
        printf("Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    if (a >= b) {
        printf("Interval start must be less than interval end\n");
        return EXIT_FAILURE;
    }
    
    printf("\nFinding roots in interval [%g, %g]:\n", a, b);
    
    double root1 = find_root_bisection(compute_polynomial, a, b);
    if (!isnan(root1)) {
        printf("Polynomial root: %g (f(x) = %g)\n", root1, compute_polynomial(root1));
    } else {
        printf("No polynomial root found in interval\n");
    }
    
    double root2 = find_root_bisection(compute_exponential, a, b);
    if (!isnan(root2)) {
        printf("Exponential root: %g (f(x) = %g)\n", root2, compute_exponential(root2));
    } else {
        printf("No exponential root found in interval\n");
    }
    
    double root3 = find_root_bisection(compute_trigonometric, a, b);
    if (!isnan(root3)) {
        printf("Trigonometric root: %g (f(x) = %g)\n", root3, compute_trigonometric(root3));
    } else {
        printf("No trigonometric root found in interval\n");
    }
    
    printf("\nFunction evaluations at midpoint %g:\n", (a + b) / 2.0);
    double mid = (a + b) / 2.0;
    printf("Polynomial: %g\n", compute_polynomial(mid));
    printf("Exponential: %g\n", compute_exponential(mid));
    printf("Trigonometric: %g\n", compute_trigonometric(mid));
    
    return EXIT_SUCCESS;
}