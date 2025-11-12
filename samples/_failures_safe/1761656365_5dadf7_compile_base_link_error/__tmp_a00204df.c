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

double find_root_bisection(double a, double b) {
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b)) {
        return NAN;
    }
    
    if (compute_polynomial(a) * compute_polynomial(b) >= 0) {
        return NAN;
    }
    
    double c = a;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        c = (a + b) / 2.0;
        
        if (fabs(compute_polynomial(c)) < EPSILON) {
            break;
        }
        
        if (compute_polynomial(c) * compute_polynomial(a) < 0) {
            b = c;
        } else {
            a = c;
        }
    }
    
    return c;
}

int main() {
    double input_value;
    
    printf("Enter a numeric value between %.1f and %.1f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        printf("Error: Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input_value)) {
        printf("Error: Input value out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    double poly_result = compute_polynomial(input_value);
    double exp_result = compute_exponential(input_value);
    
    printf("Polynomial f(x) = 3x^2 - 2x + 5 at x = %.6f: %.6f\n", input_value, poly_result);
    printf("Exponential g(x) = exp(x/2) at x = %.6f: %.6f\n", input_value, exp_result);
    
    double root_a = -10.0;
    double root_b = 10.0;
    double root = find_root_bisection(root_a, root_b);
    
    if (!isnan(root)) {
        printf("Root of polynomial in [%.1f, %.1f]: %.6f\n", root_a, root_b, root);
        printf("Verification f(root) = %.6f\n", compute_polynomial(root));
    } else {
        printf("No root found in the interval [%.1f, %.1f]\n", root_a, root_b);
    }
    
    double combined_result = poly_result + exp_result;
    printf("Combined result (polynomial + exponential): %.6f\n", combined_result);
    
    return EXIT_SUCCESS;
}