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
    int iter = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iter++;
    } while (ABS(x - prev) > EPSILON && iter < MAX_ITERATIONS);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) {
        if (base == 0) return NAN;
        base = 1.0 / base;
        exponent = -exponent;
    }
    
    double result = 1.0;
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

int main() {
    double input_value;
    int power;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input_value)) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter an integer power (-100 to 100): ");
    if (scanf("%d", &power) != 1) {
        fprintf(stderr, "Error: Invalid power input\n");
        return EXIT_FAILURE;
    }
    
    if (power < -100 || power > 100) {
        fprintf(stderr, "Error: Power out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_value);
    double power_result = compute_power(input_value, power);
    
    if (isnan(sqrt_result)) {
        printf("Square root of %.2f is undefined\n", input_value);
    } else {
        printf("Square root of %.2f = %.6f\n", input_value, sqrt_result);
    }
    
    if (isnan(power_result)) {
        printf("Power operation %.2f^%d is undefined\n", input_value, power);
    } else {
        printf("Power result %.2f^%d = %.6f\n", input_value, power, power_result);
    }
    
    double reference_sqrt = sqrt(input_value);
    double reference_power = pow(input_value, power);
    
    printf("Reference sqrt: %.6f\n", reference_sqrt);
    printf("Reference power: %.6f\n", reference_power);
    
    return EXIT_SUCCESS;
}