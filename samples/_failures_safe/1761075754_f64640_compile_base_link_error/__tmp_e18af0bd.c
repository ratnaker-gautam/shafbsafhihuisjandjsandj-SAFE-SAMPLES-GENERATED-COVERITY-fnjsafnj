//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double safe_sqrt(double x) {
    if (x < 0.0) return NAN;
    return sqrt(x);
}

double safe_log(double x) {
    if (x <= 0.0) return NAN;
    return log(x);
}

double compute_function(double x) {
    double term1 = safe_sqrt(x);
    double term2 = safe_log(x + 1.0);
    if (isnan(term1) || isnan(term2)) return NAN;
    return term1 * term2;
}

double numerical_integration(double a, double b, int n) {
    if (a >= b || n <= 0 || n > MAX_ITERATIONS) return NAN;
    if (a < MIN_INPUT || b > MAX_INPUT) return NAN;
    
    double h = (b - a) / n;
    double sum = 0.0;
    
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        double fx = compute_function(x);
        if (isnan(fx)) return NAN;
        sum += fx;
    }
    
    return sum * h;
}

int main(void) {
    double lower_bound, upper_bound;
    int segments;
    
    printf("Enter lower bound (>= %.10g): ", MIN_INPUT);
    if (scanf("%lf", &lower_bound) != 1) {
        printf("Invalid input for lower bound\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter upper bound (<= %.0f): ", MAX_INPUT);
    if (scanf("%lf", &upper_bound) != 1) {
        printf("Invalid input for upper bound\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of segments (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &segments) != 1) {
        printf("Invalid input for segments\n");
        return EXIT_FAILURE;
    }
    
    if (lower_bound < MIN_INPUT || upper_bound > MAX_INPUT || lower_bound >= upper_bound) {
        printf("Invalid bounds: must have %.10g <= lower < upper <= %.0f\n", 
               MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    if (segments <= 0 || segments > MAX_ITERATIONS) {
        printf("Invalid segments: must be between 1 and %d\n", MAX_ITERATIONS);
        return EXIT_FAILURE;
    }
    
    double result = numerical_integration(lower_bound, upper_bound, segments);
    
    if (isnan(result)) {
        printf("Computation failed: function undefined in range or numerical error\n");
        return EXIT_FAILURE;
    }
    
    printf("Numerical integration result: %.10g\n", result);
    
    return EXIT_SUCCESS;
}