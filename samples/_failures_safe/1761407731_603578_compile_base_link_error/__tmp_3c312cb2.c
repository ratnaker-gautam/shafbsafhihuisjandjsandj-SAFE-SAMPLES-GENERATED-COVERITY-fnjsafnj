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
        if (exponent % 2 == 1) {
            result *= base;
        }
        base *= base;
        exponent /= 2;
    }
    return result;
}

int main(void) {
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
    
    printf("Input value: %f\n", input_value);
    printf("Square root (Newton): %f\n", sqrt_result);
    printf("Value raised to power %d: %f\n", power, power_result);
    
    if (input_value >= 0) {
        double std_sqrt = sqrt(input_value);
        printf("Standard library sqrt: %f\n", std_sqrt);
        printf("Difference: %e\n", ABS(sqrt_result - std_sqrt));
    }
    
    return EXIT_SUCCESS;
}