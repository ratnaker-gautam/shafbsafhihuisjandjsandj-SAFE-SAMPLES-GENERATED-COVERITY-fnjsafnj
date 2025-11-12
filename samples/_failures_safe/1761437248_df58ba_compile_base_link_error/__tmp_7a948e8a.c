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
    if (!VALIDATE_INPUT(x)) return -1.0;
    
    double result = 1.0;
    double term = 1.0;
    int n = 1;
    
    while (n <= 50 && ABS(term) > EPSILON) {
        term *= x / n;
        result += term;
        n++;
        if (!VALIDATE_INPUT(result)) return -1.0;
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
    
    if (sscanf(buffer, "%lf", &input) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(input)) {
        fprintf(stderr, "Input out of valid range\n");
        return 1;
    }
    
    double sqrt_result = compute_sqrt_newton(input);
    double log_result = compute_log_approx(input);
    double exp_result = compute_exp_taylor(input);
    
    printf("Input: %f\n", input);
    
    if (sqrt_result >= 0) {
        printf("Square root (Newton): %f\n", sqrt_result);
        printf("Square root (library): %f\n", sqrt(input));
    } else {
        printf("Square root: Invalid input\n");
    }
    
    if (log_result >= 0) {
        printf("Natural log (approx): %f\n", log_result);
        printf("Natural log (library): %f\n", log(input));
    } else {
        printf("Natural log: Invalid input\n");
    }
    
    if (exp_result >= 0) {
        printf("Exponential (Taylor): %f\n", exp_result);
        printf("Exponential (library): %f\n", exp(input));
    } else {
        printf("Exponential: Invalid input\n");
    }
    
    return 0;
}