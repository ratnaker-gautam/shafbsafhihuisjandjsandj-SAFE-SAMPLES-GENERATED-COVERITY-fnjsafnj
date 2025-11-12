//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
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
    if (x < 0.0) {
        return NAN;
    }
    return sqrt(x);
}

double safe_log(double x) {
    if (x <= 0.0) {
        return NAN;
    }
    return log(x);
}

double compute_function(double x) {
    double sqrt_val = safe_sqrt(x);
    if (isnan(sqrt_val)) {
        return NAN;
    }
    double log_val = safe_log(x);
    if (isnan(log_val)) {
        return NAN;
    }
    return sqrt_val * log_val;
}

double find_root_bisection(double a, double b) {
    if (a >= b) {
        return NAN;
    }
    double fa = compute_function(a);
    double fb = compute_function(b);
    if (isnan(fa) || isnan(fb)) {
        return NAN;
    }
    if (fa * fb > 0.0) {
        return NAN;
    }
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double mid = a + (b - a) / 2.0;
        double fmid = compute_function(mid);
        if (isnan(fmid)) {
            return NAN;
        }
        if (fabs(fmid) < TOLERANCE || (b - a) / 2.0 < TOLERANCE) {
            return mid;
        }
        if (fa * fmid < 0.0) {
            b = mid;
            fb = fmid;
        } else {
            a = mid;
            fa = fmid;
        }
    }
    return NAN;
}

int main() {
    double lower, upper;
    printf("Enter lower bound for root finding: ");
    if (scanf("%lf", &lower) != 1) {
        printf("Invalid input for lower bound.\n");
        return EXIT_FAILURE;
    }
    printf("Enter upper bound for root finding: ");
    if (scanf("%lf", &upper) != 1) {
        printf("Invalid input for upper bound.\n");
        return EXIT_FAILURE;
    }
    if (lower >= upper) {
        printf("Lower bound must be less than upper bound.\n");
        return EXIT_FAILURE;
    }
    if (fabs(lower) > MAX_INPUT || fabs(upper) > MAX_INPUT) {
        printf("Input values too large.\n");
        return EXIT_FAILURE;
    }
    double root = find_root_bisection(lower, upper);
    if (isnan(root)) {
        printf("No root found in the given interval or invalid interval.\n");
        return EXIT_FAILURE;
    }
    printf("Approximate root: %.10f\n", root);
    printf("Function value at root: %.10f\n", compute_function(root));
    return EXIT_SUCCESS;
}