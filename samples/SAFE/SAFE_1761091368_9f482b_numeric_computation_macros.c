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
#define IS_VALID(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)

double newton_sqrt(double n) {
    if (n < 0.0) return -1.0;
    if (n == 0.0) return 0.0;
    double x = n;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = x * x - n;
        if (fabs(fx) < EPSILON) break;
        double dfx = 2.0 * x;
        if (fabs(dfx) < EPSILON) break;
        double delta = fx / dfx;
        x = x - delta;
        if (x < 0.0) x = -x;
    }
    return x;
}

double safe_power(double base, int exp) {
    if (exp == 0) return 1.0;
    if (base == 0.0) return 0.0;
    double result = 1.0;
    int abs_exp = exp < 0 ? -exp : exp;
    for (int i = 0; i < abs_exp; i++) {
        if (result > MAX_INPUT / fabs(base)) return MAX_INPUT;
        result *= base;
    }
    if (exp < 0) {
        if (fabs(result) < 1.0 / MAX_INPUT) return MAX_INPUT;
        result = 1.0 / result;
    }
    return result;
}

int main(void) {
    double input_num;
    int input_exp;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_num) != 1) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(input_num)) {
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
    
    double sqrt_result = newton_sqrt(input_num);
    if (sqrt_result < 0.0) {
        printf("Cannot compute square root of negative number.\n");
    } else {
        printf("Square root: %.10f\n", sqrt_result);
    }
    
    double power_result = safe_power(input_num, input_exp);
    printf("Power result: %.10f\n", power_result);
    
    double combined = sqrt_result + power_result;
    if (!IS_VALID(combined)) {
        printf("Combined result exceeds computational limits.\n");
        return EXIT_FAILURE;
    }
    
    printf("Combined result: %.10f\n", combined);
    
    return EXIT_SUCCESS;
}