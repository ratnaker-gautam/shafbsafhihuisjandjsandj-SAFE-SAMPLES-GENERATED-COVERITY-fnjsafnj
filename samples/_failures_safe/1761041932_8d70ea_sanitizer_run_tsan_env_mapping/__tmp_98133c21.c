//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
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
        return 1.0 / compute_power(base, -exponent);
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
        printf("Exponent out of range.\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_num);
    double power_result = compute_power(input_num, input_exp);
    
    printf("Square root: ");
    if (isnan(sqrt_result)) {
        printf("Undefined for negative numbers\n");
    } else {
        printf("%.10f\n", sqrt_result);
    }
    
    printf("Power result: ");
    if (isnan(power_result)) {
        printf("Undefined (division by zero)\n");
    } else {
        printf("%.10f\n", power_result);
    }
    
    return EXIT_SUCCESS;
}