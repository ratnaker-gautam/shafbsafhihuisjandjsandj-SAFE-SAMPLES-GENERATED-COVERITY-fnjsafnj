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
        if (ABS(prev) < EPSILON) break;
        x = 0.5 * (prev + n / prev);
        iterations++;
    } while (ABS(x - prev) > EPSILON && iterations < MAX_ITERATIONS);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent == 1) return base;
    
    int abs_exp = exponent < 0 ? -exponent : exponent;
    double result = 1.0;
    
    while (abs_exp > 0) {
        if (abs_exp & 1) {
            if (result > DBL_MAX / base) return -1.0;
            result *= base;
        }
        if (abs_exp > 1) {
            if (base > DBL_MAX / base) return -1.0;
            base *= base;
        }
        abs_exp >>= 1;
    }
    
    if (exponent < 0) {
        if (result < DBL_MIN) return -1.0;
        return 1.0 / result;
    }
    return result;
}

int main() {
    double input_num;
    int exponent;
    
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
    if (scanf("%d", &exponent) != 1) {
        printf("Invalid exponent format.\n");
        return 1;
    }
    
    if (exponent < -100 || exponent > 100) {
        printf("Exponent out of valid range.\n");
        return 1;
    }
    
    double sqrt_result = compute_sqrt_newton(input_num);
    if (sqrt_result < 0) {
        printf("Cannot compute square root of negative number.\n");
    } else {
        printf("Square root: %.10f\n", sqrt_result);
    }
    
    double power_result = compute_power(input_num, exponent);
    if (power_result < 0) {
        printf("Power computation overflowed.\n");
    } else {
        printf("%.2f^%d = %.10f\n", input_num, exponent, power_result);
    }
    
    return 0;
}