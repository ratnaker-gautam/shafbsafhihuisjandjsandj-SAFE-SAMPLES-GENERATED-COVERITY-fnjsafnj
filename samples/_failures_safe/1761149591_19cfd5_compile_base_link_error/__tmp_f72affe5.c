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
    if (n < 0.0) return -1.0;
    if (n == 0.0) return 0.0;
    
    double x = n;
    double prev;
    int iterations = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iterations++;
    } while (ABS(x - prev) > EPSILON && iterations < MAX_ITERATIONS);
    
    return x;
}

double compute_log_approx(double n) {
    if (n <= 0.0) return -1.0;
    
    double result = 0.0;
    double term = (n - 1.0) / (n + 1.0);
    double term_sq = term * term;
    double current_term = term;
    
    for (int i = 1; i <= 50; i++) {
        result += current_term / (2 * i - 1);
        current_term *= term_sq;
        if (ABS(current_term) < EPSILON) break;
    }
    
    return 2.0 * result;
}

int main() {
    double input;
    char buffer[256];
    
    printf("Enter a positive number: ");
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
        fprintf(stderr, "Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return 1;
    }
    
    if (input <= 0.0) {
        fprintf(stderr, "Input must be positive\n");
        return 1;
    }
    
    double sqrt_result = compute_sqrt_newton(input);
    double log_result = compute_log_approx(input);
    
    if (sqrt_result < 0.0 || log_result < 0.0) {
        fprintf(stderr, "Computation error\n");
        return 1;
    }
    
    printf("Number: %g\n", input);
    printf("Square root (Newton): %g\n", sqrt_result);
    printf("Natural log (approx): %g\n", log_result);
    printf("Library sqrt: %g\n", sqrt(input));
    printf("Library log: %g\n", log(input));
    
    return 0;
}