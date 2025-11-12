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

double compute_log_approx(double n) {
    if (n <= 0) return -1.0;
    
    double result = 0.0;
    double term = (n - 1) / (n + 1);
    double term_sq = term * term;
    double current_term = term;
    
    for (int i = 1; i <= 50; i++) {
        result += current_term / (2 * i - 1);
        current_term *= term_sq;
        if (ABS(current_term) < EPSILON) break;
    }
    
    return 2.0 * result;
}

double compute_power_series(double x, int n) {
    if (n < 0) return -1.0;
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
    int choice;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(input_value)) {
        printf("Input out of valid range\n");
        return 1;
    }
    
    printf("Choose computation:\n");
    printf("1. Square root (Newton's method)\n");
    printf("2. Natural logarithm (approximation)\n");
    printf("3. Exponential series (e^x approximation)\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice format\n");
        return 1;
    }
    
    double result;
    
    switch (choice) {
        case 1:
            result = compute_sqrt_newton(input_value);
            if (result < 0) {
                printf("Cannot compute square root of negative number\n");
                return 1;
            }
            printf("Square root: %.10f\n", result);
            printf("Reference sqrt: %.10f\n", sqrt(input_value));
            break;
            
        case 2:
            result = compute_log_approx(input_value);
            if (result < 0) {
                printf("Cannot compute logarithm of non-positive number\n");
                return 1;
            }
            printf("Natural logarithm: %.10f\n", result);
            printf("Reference log: %.10f\n", log(input_value));
            break;
            
        case 3:
            result = compute_power_series(input_value, 50);
            printf("Exponential approximation: %.10f\n", result);
            printf("Reference exp: %.10f\n", exp(input_value));
            break;
            
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}