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
    int iter = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iter++;
    } while (ABS(x - prev) > EPSILON && iter < MAX_ITERATIONS);
    
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
    
    return 2 * result;
}

double compute_exp_taylor(double x) {
    if (x > 100.0 || x < -100.0) return -1.0;
    
    double result = 1.0;
    double term = 1.0;
    
    for (int i = 1; i <= 100; i++) {
        term *= x / i;
        result += term;
        if (ABS(term) < EPSILON) break;
    }
    
    return result;
}

int main() {
    double input;
    char buffer[256];
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    input = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(input)) {
        fprintf(stderr, "Input out of range\n");
        return 1;
    }
    
    double sqrt_result = compute_sqrt_newton(input);
    double log_result = compute_log_approx(input);
    double exp_result = compute_exp_taylor(input);
    
    if (sqrt_result >= 0) {
        printf("Square root: %.10f\n", sqrt_result);
    } else {
        printf("Square root: undefined for negative input\n");
    }
    
    if (log_result >= 0) {
        printf("Natural logarithm: %.10f\n", log_result);
    } else {
        printf("Natural logarithm: undefined for non-positive input\n");
    }
    
    if (exp_result >= 0) {
        printf("Exponential: %.10f\n", exp_result);
    } else {
        printf("Exponential: input out of range for approximation\n");
    }
    
    double verification = exp_result * log_result;
    if (sqrt_result >= 0 && log_result >= 0 && exp_result >= 0) {
        printf("Verification (exp * ln): %.10f\n", verification);
    }
    
    return 0;
}