//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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
        if (x == 0.0) return 0.0;
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

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0) return 0.0;
    
    double result = 1.0;
    int abs_exp = ABS(exponent);
    double abs_base = ABS(base);
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > DBL_MAX / abs_base) {
            return exponent > 0 ? INFINITY : 0.0;
        }
        result *= abs_base;
    }
    
    if (base < 0 && (abs_exp % 2 == 1)) {
        result = -result;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}

int main(void) {
    double input_num;
    int input_exp;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_num) != 1) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input_num)) {
        printf("Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter an integer exponent (-100 to 100): ");
    if (scanf("%d", &input_exp) != 1) {
        printf("Invalid exponent format.\n");
        return EXIT_FAILURE;
    }
    
    if (input_exp < -100 || input_exp > 100) {
        printf("Exponent out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_num);
    double log_result = compute_log_approx(input_num);
    double power_result = compute_power(input_num, input_exp);
    
    printf("Square root: ");
    if (isnan(sqrt_result)) {
        printf("Undefined for negative input\n");
    } else {
        printf("%.10f\n", sqrt_result);
    }
    
    printf("Natural logarithm: ");
    if (isnan(log_result)) {
        printf("Undefined for non-positive input\n");
    } else {
        printf("%.10f\n", log_result);
    }
    
    printf("Power result: ");
    if (isinf(power_result)) {
        printf("Overflow occurred\n");
    } else if (power_result == 0.0 && input_num != 0.0) {
        printf("Underflow occurred\n");
    } else {
        printf("%.10f\n", power_result);
    }
    
    return EXIT_SUCCESS;
}