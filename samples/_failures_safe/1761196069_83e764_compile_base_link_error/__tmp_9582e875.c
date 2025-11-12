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
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_sqrt_newton(double n) {
    if (n < 0) return NAN;
    if (n == 0) return 0.0;
    
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

double compute_log_approx(double n) {
    if (n <= 0) return NAN;
    
    double result = 0.0;
    double term = (n - 1) / (n + 1);
    double term_sq = term * term;
    double current_term = term;
    
    for (int i = 1; i <= 50; i++) {
        result += current_term / (2 * i - 1);
        current_term *= term_sq;
        if (ABS(current_term) < EPSILON) break;
    }
    
    return 2 * result;
}

double compute_power_series(double x, int n) {
    if (n < 0) return NAN;
    if (n == 0) return 1.0;
    
    double result = 1.0;
    double term = 1.0;
    
    for (int i = 1; i <= n; i++) {
        term *= x / i;
        result += term;
        if (ABS(term) < EPSILON) break;
    }
    
    return result;
}

int main() {
    double input_value;
    int power;
    
    printf("Enter a numeric value between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input_value)) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter power for series expansion (0-100): ");
    if (scanf("%d", &power) != 1) {
        fprintf(stderr, "Error: Invalid power input\n");
        return EXIT_FAILURE;
    }
    
    if (power < 0 || power > 100) {
        fprintf(stderr, "Error: Power out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_value);
    double log_result = compute_log_approx(input_value);
    double power_result = compute_power_series(input_value, power);
    
    printf("Input value: %f\n", input_value);
    
    if (!isnan(sqrt_result)) {
        printf("Square root (Newton): %f\n", sqrt_result);
        printf("Library sqrt: %f\n", sqrt(input_value));
    } else {
        printf("Square root: Invalid input (negative)\n");
    }
    
    if (!isnan(log_result)) {
        printf("Natural log (approx): %f\n", log_result);
        printf("Library log: %f\n", log(input_value));
    } else {
        printf("Natural log: Invalid input (non-positive)\n");
    }
    
    if (!isnan(power_result)) {
        printf("Exponential series (n=%d): %f\n", power, power_result);
        printf("Library exp: %f\n", exp(input_value));
    } else {
        printf("Exponential series: Invalid power\n");
    }
    
    return EXIT_SUCCESS;
}