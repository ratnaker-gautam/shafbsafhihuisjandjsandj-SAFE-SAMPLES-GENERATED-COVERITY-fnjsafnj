//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_sqrt_newton(double x) {
    if (x < 0.0) return NAN;
    if (x == 0.0) return 0.0;
    
    double guess = x / 2.0;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double new_guess = 0.5 * (guess + x / guess);
        if (fabs(new_guess - guess) < TOLERANCE) {
            return new_guess;
        }
        guess = new_guess;
    }
    return guess;
}

double compute_log_approx(double x) {
    if (x <= 0.0) return NAN;
    
    double term = (x - 1.0) / (x + 1.0);
    double term_sq = term * term;
    double sum = term;
    double current_term = term;
    
    for (int i = 1; i < 100; i++) {
        current_term *= term_sq;
        double new_sum = sum + current_term / (2 * i + 1);
        if (new_sum == sum) break;
        sum = new_sum;
    }
    return 2.0 * sum;
}

double compute_power_series(double x, int n) {
    if (n < 0) return NAN;
    if (n == 0) return 1.0;
    
    double result = 1.0;
    double term = 1.0;
    
    for (int i = 1; i <= n; i++) {
        term *= x / i;
        double new_result = result + term;
        if (new_result == result) break;
        result = new_result;
    }
    return result;
}

int main(void) {
    double input_value;
    int choice;
    
    printf("Enter a positive number: ");
    if (scanf("%lf", &input_value) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (input_value <= 0.0 || input_value > MAX_INPUT) {
        printf("Input out of range (0, %d]\n", (int)MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    printf("Choose computation:\n");
    printf("1. Square root (Newton's method)\n");
    printf("2. Natural logarithm (approximation)\n");
    printf("3. Exponential series\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return EXIT_FAILURE;
    }
    
    double result;
    switch (choice) {
        case 1:
            result = compute_sqrt_newton(input_value);
            if (isnan(result)) {
                printf("Cannot compute square root of negative number\n");
                return EXIT_FAILURE;
            }
            printf("Square root: %.10f\n", result);
            printf("Library sqrt: %.10f\n", sqrt(input_value));
            break;
            
        case 2:
            if (input_value < MIN_INPUT) {
                printf("Input too small for logarithm\n");
                return EXIT_FAILURE;
            }
            result = compute_log_approx(input_value);
            if (isnan(result)) {
                printf("Cannot compute logarithm\n");
                return EXIT_FAILURE;
            }
            printf("Natural log: %.10f\n", result);
            printf("Library log: %.10f\n", log(input_value));
            break;
            
        case 3:
            if (input_value > 100.0) {
                printf("Input too large for series\n");
                return EXIT_FAILURE;
            }
            result = compute_power_series(input_value, 50);
            if (isnan(result)) {
                printf("Cannot compute exponential\n");
                return EXIT_FAILURE;
            }
            printf("Exponential: %.10f\n", result);
            printf("Library exp: %.10f\n", exp(input_value));
            break;
            
        default:
            printf("Invalid choice\n");
            return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}