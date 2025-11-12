//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define IS_VALID_INPUT(x) ((x) >= 0.0 && (x) <= 100.0)
#define CLAMP(x, min, max) (((x) < (min)) ? (min) : (((x) > (max)) ? (max) : (x)))
#define SQUARE(x) ((x) * (x))

double compute_sqrt_newton(double n) {
    if (n < 0.0) return -1.0;
    if (n == 0.0) return 0.0;
    double x = n;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (x == 0.0) return 0.0;
        double next_x = 0.5 * (x + n / x);
        if (fabs(next_x - x) < EPSILON) return next_x;
        x = next_x;
    }
    return x;
}

double compute_compound_interest(double principal, double rate, int years) {
    if (principal < 0.0 || rate < 0.0 || years < 0) return -1.0;
    if (principal == 0.0) return 0.0;
    double amount = principal;
    for (int i = 0; i < years; i++) {
        if (amount > DBL_MAX / (1.0 + rate)) return -1.0;
        amount *= (1.0 + rate);
        if (amount < principal) return -1.0;
    }
    return amount;
}

int main(void) {
    double input_value;
    printf("Enter a number between 0 and 100 to compute its square root: ");
    if (scanf("%lf", &input_value) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    if (!IS_VALID_INPUT(input_value)) {
        printf("Input out of range.\n");
        return 1;
    }
    double sqrt_result = compute_sqrt_newton(input_value);
    if (sqrt_result < 0.0) {
        printf("Error computing square root.\n");
        return 1;
    }
    printf("Square root of %.2f is approximately %.6f\n", input_value, sqrt_result);
    double principal, rate;
    int years;
    printf("Enter principal amount (0-1000000): ");
    if (scanf("%lf", &principal) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    principal = CLAMP(principal, 0.0, 1000000.0);
    printf("Enter annual interest rate (0-1.0): ");
    if (scanf("%lf", &rate) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    rate = CLAMP(rate, 0.0, 1.0);
    printf("Enter number of years (0-100): ");
    if (scanf("%d", &years) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    years = CLAMP(years, 0, 100);
    double interest_result = compute_compound_interest(principal, rate, years);
    if (interest_result < 0.0) {
        printf("Error computing compound interest.\n");
        return 1;
    }
    printf("Compound interest after %d years: %.2f\n", years, interest_result);
    return 0;
}