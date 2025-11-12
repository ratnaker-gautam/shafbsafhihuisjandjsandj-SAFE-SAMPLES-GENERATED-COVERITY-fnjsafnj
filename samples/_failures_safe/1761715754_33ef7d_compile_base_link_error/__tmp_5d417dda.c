//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 0.0001

double safe_sqrt(double x) {
    if (x < 0.0) return NAN;
    return sqrt(x);
}

double safe_pow(double base, double exp) {
    if (base == 0.0 && exp <= 0.0) return NAN;
    if (base < 0.0 && floor(exp) != exp) return NAN;
    return pow(base, exp);
}

int main(void) {
    double input_value;
    char buffer[256];
    
    printf("Enter a positive number (0.0001 to 1000000): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    input_value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Input out of range\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = safe_sqrt(input_value);
    if (isnan(sqrt_result)) {
        fprintf(stderr, "Square root calculation failed\n");
        return EXIT_FAILURE;
    }
    
    double log_result = log(input_value);
    if (isinf(log_result) || isnan(log_result)) {
        fprintf(stderr, "Logarithm calculation failed\n");
        return EXIT_FAILURE;
    }
    
    double pow_result = safe_pow(input_value, 2.5);
    if (isnan(pow_result) || isinf(pow_result)) {
        fprintf(stderr, "Power calculation failed\n");
        return EXIT_FAILURE;
    }
    
    double newton_approx = input_value / 2.0;
    int iterations = 0;
    while (iterations < MAX_ITERATIONS) {
        double prev = newton_approx;
        newton_approx = 0.5 * (newton_approx + input_value / newton_approx);
        if (fabs(newton_approx - prev) < TOLERANCE) break;
        iterations++;
    }
    
    printf("Input: %.6f\n", input_value);
    printf("Square root: %.6f\n", sqrt_result);
    printf("Natural logarithm: %.6f\n", log_result);
    printf("Power (x^2.5): %.6f\n", pow_result);
    printf("Newton sqrt approximation: %.6f\n", newton_approx);
    printf("Newton iterations: %d\n", iterations);
    
    double harmonic_sum = 0.0;
    int n = (int)fmin(1000, input_value);
    if (n < 1) n = 1;
    for (int i = 1; i <= n; i++) {
        harmonic_sum += 1.0 / i;
    }
    printf("Harmonic sum (n=%d): %.6f\n", n, harmonic_sum);
    
    return EXIT_SUCCESS;
}