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
    if (n < 0) return -1.0;
    if (n == 0.0) return 0.0;
    
    double x = n;
    double prev;
    int iterations = 0;
    
    do {
        prev = x;
        if (x == 0.0) break;
        x = 0.5 * (x + n / x);
        iterations++;
        if (iterations > MAX_ITERATIONS) break;
    } while (ABS(x - prev) > EPSILON);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent == 1) return base;
    
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < abs_exp; i++) {
        if (ABS(base) > 1.0 && result > DBL_MAX / ABS(base)) {
            return exponent > 0 ? DBL_MAX : 0.0;
        }
        if (ABS(base) < 1.0 && result < DBL_MIN / ABS(base)) {
            return exponent > 0 ? 0.0 : DBL_MAX;
        }
        result *= base;
    }
    
    if (exponent < 0) {
        if (result == 0.0) return DBL_MAX;
        return 1.0 / result;
    }
    return result;
}

int main() {
    double input_num;
    int input_exp;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_num) != 1) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(input_num)) {
        printf("Input out of valid range.\n");
        return 1;
    }
    
    printf("Enter an integer exponent (-100 to 100): ");
    if (scanf("%d", &input_exp) != 1) {
        printf("Invalid exponent format.\n");
        return 1;
    }
    
    if (input_exp < -100 || input_exp > 100) {
        printf("Exponent out of valid range.\n");
        return 1;
    }
    
    double sqrt_result = compute_sqrt_newton(input_num);
    if (sqrt_result < 0) {
        printf("Cannot compute square root of negative number.\n");
    } else {
        printf("Square root: %.10f\n", sqrt_result);
    }
    
    double power_result = compute_power(input_num, input_exp);
    printf("Power result: %.10f\n", power_result);
    
    double combined = compute_power(sqrt_result, input_exp);
    printf("Square root raised to exponent: %.10f\n", combined);
    
    return 0;
}