//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series(double x, int* iterations) {
    if (x <= 0.0 || x > MAX_INPUT) {
        return -1.0;
    }
    
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= MAX_ITERATIONS && fabs(term) > TOLERANCE) {
        result += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        
        if (n == INT_MAX) {
            break;
        }
        n++;
    }
    
    *iterations = n - 1;
    return result;
}

int main(void) {
    double input;
    char buffer[100];
    
    printf("Enter a positive number (1e-10 to 1000000): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* endptr;
    input = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (input <= 0.0 || input > MAX_INPUT || input < MIN_INPUT) {
        fprintf(stderr, "Input out of valid range\n");
        return 1;
    }
    
    if (!isfinite(input)) {
        fprintf(stderr, "Input is not finite\n");
        return 1;
    }
    
    int iterations;
    double result = compute_series(input, &iterations);
    
    if (result < 0.0) {
        fprintf(stderr, "Computation failed\n");
        return 1;
    }
    
    double reference = sin(input) / input;
    
    printf("Input: %.10f\n", input);
    printf("Series result: %.10f\n", result);
    printf("Reference (sin(x)/x): %.10f\n", reference);
    printf("Absolute error: %.2e\n", fabs(result - reference));
    printf("Iterations used: %d\n", iterations);
    
    return 0;
}