//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
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
        x = 0.5 * (x + n / x);
        iterations++;
        if (iterations > MAX_ITERATIONS) break;
    } while (ABS(x - prev) > TOLERANCE);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent == 1) return base;
    
    int abs_exp = exponent < 0 ? -exponent : exponent;
    double result = 1.0;
    
    while (abs_exp > 0) {
        if (abs_exp & 1) {
            result *= base;
        }
        base *= base;
        abs_exp >>= 1;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}

double compute_compound_interest(double principal, double rate, int years) {
    if (principal < 0 || rate < 0 || years < 0) return -1.0;
    if (years == 0) return principal;
    
    double factor = 1.0 + rate / 100.0;
    return principal * compute_power(factor, years);
}

int main(void) {
    double principal, rate;
    int years;
    
    printf("Enter principal amount: ");
    if (scanf("%lf", &principal) != 1) {
        printf("Invalid input for principal\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(principal) || principal < 0) {
        printf("Principal must be between 0 and %f\n", MAX_INPUT);
        return 1;
    }
    
    printf("Enter annual interest rate (%%): ");
    if (scanf("%lf", &rate) != 1) {
        printf("Invalid input for rate\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(rate) || rate < 0) {
        printf("Rate must be between 0 and %f\n", MAX_INPUT);
        return 1;
    }
    
    printf("Enter number of years: ");
    if (scanf("%d", &years) != 1) {
        printf("Invalid input for years\n");
        return 1;
    }
    
    if (years < 0 || years > 100) {
        printf("Years must be between 0 and 100\n");
        return 1;
    }
    
    double future_value = compute_compound_interest(principal, rate, years);
    
    if (future_value < 0) {
        printf("Error in calculation\n");
        return 1;
    }
    
    printf("Future value: %.2f\n", future_value);
    
    double sqrt_val = compute_sqrt_newton(future_value);
    if (sqrt_val >= 0) {
        printf("Square root of future value: %.6f\n", sqrt_val);
    } else {
        printf("Cannot compute square root of negative value\n");
    }
    
    return 0;
}