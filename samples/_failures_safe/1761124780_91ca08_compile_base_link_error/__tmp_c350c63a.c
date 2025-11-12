//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 0.0001

double compute_series(double x, int* iterations) {
    if (x <= 0.0 || x > MAX_INPUT) {
        return -1.0;
    }
    
    double sum = 0.0;
    double term = 1.0;
    int n = 0;
    
    while (n < MAX_ITERATIONS && fabs(term) > TOLERANCE) {
        sum += term;
        n++;
        term = term * (-x) / n;
    }
    
    if (iterations != NULL) {
        *iterations = n;
    }
    
    return sum;
}

int main(void) {
    double input;
    char buffer[256];
    
    printf("Enter a positive number (0.0001 to 1000000): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char* endptr;
    input = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input <= MIN_INPUT || input > MAX_INPUT) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    if (input != input) {
        fprintf(stderr, "Input is not a valid number\n");
        return EXIT_FAILURE;
    }
    
    int iterations;
    double result = compute_series(input, &iterations);
    
    if (result < 0.0) {
        fprintf(stderr, "Computation error\n");
        return EXIT_FAILURE;
    }
    
    printf("Series approximation of e^(-%.6f): %.10f\n", input, result);
    printf("Converged after %d iterations\n", iterations);
    
    double exact = exp(-input);
    double error = fabs(result - exact);
    printf("Exact value: %.10f\n", exact);
    printf("Absolute error: %.2e\n", error);
    
    return EXIT_SUCCESS;
}