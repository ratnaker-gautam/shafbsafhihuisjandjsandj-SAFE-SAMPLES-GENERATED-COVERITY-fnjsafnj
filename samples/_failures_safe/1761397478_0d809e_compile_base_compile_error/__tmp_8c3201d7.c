//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double safe_power(double base, int exponent) {
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    if (base == 0.0 && exponent <= 0) {
        return NAN;
    }
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > DBL_MAX / fabs(base)) {
            return exponent > 0 ? (base > 0 ? DBL_MAX : -DBL_MAX) : 0.0;
        }
        result *= base;
    }
    
    if (exponent < 0) {
        if (fabs(result) < 1e-300) {
            return base > 0 ? DBL_MAX : -DBL_MAX;
        }
        result = 1.0 / result;
    }
    
    return result;
}

double compute_series(double x, int *iterations) {
    double sum = 0.0;
    double term;
    int n = 0;
    
    do {
        if (n >= MAX_ITERATIONS) {
            break;
        }
        
        double numerator = safe_power(-1, n);
        double denominator = 2 * n + 1;
        term = numerator / denominator * safe_power(x, 2 * n + 1);
        
        if (!isfinite(term)) {
            break;
        }
        
        sum += term;
        n++;
    } while (fabs(term) > EPSILON);
    
    *iterations = n;
    return 4.0 * sum;
}

int main(void) {
    double input;
    char buffer[100];
    
    printf("Enter a value between %.10g and %.10g: ", MIN_INPUT, MAX_INPUT);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    input = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input < MIN_INPUT || input > MAX_INPUT) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    if (fabs(input) >= 1.0) {
        fprintf(stderr, "Input must satisfy |x| < 1 for series convergence\n");
        return EXIT_FAILURE;
    }
    
    int iterations;
    double result = compute_series(input, &iterations);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Numerical computation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("arctan(%.10g) ≈ %.10g\n", input, result);
    printf("Computed in %d iterations\n", iterations);
    printf("Reference value: %.10g\n", atan(input));
    printf("Absolute error: %.10g\n", fabs(result - atan(input)));
    
    return EXIT_SUCCESS;
}