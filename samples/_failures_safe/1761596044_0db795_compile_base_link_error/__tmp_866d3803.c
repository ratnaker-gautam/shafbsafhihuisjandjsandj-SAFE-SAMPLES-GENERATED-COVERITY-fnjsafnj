//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series(double x, int* iterations) {
    double term = x;
    double sum = term;
    int i = 1;
    
    while (i < MAX_ITERATIONS && fabs(term) > EPSILON) {
        term = -term * x * x / ((2 * i) * (2 * i + 1));
        if (fabs(term) > 1e308 || isnan(term)) {
            break;
        }
        sum += term;
        i++;
    }
    
    *iterations = i;
    return sum;
}

int main(void) {
    double input;
    char buffer[256];
    
    printf("Enter a value for x (between %.10g and %.10g): ", MIN_INPUT, MAX_INPUT);
    
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
    
    if (input < MIN_INPUT || input > MAX_INPUT) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    if (isnan(input) || isinf(input)) {
        fprintf(stderr, "Input is not a finite number\n");
        return EXIT_FAILURE;
    }
    
    int iterations;
    double result = compute_series(input, &iterations);
    
    if (isnan(result) || isinf(result)) {
        fprintf(stderr, "Computation resulted in invalid value\n");
        return EXIT_FAILURE;
    }
    
    printf("Result: %.15g\n", result);
    printf("Iterations used: %d\n", iterations);
    printf("Reference sin(x): %.15g\n", sin(input));
    printf("Difference: %.15g\n", fabs(result - sin(input)));
    
    return EXIT_SUCCESS;
}