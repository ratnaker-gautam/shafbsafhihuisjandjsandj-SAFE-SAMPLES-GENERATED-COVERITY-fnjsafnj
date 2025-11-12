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
    double sum = 0.0;
    double term = x;
    int n = 1;
    
    while (fabs(term) > TOLERANCE && n <= MAX_ITERATIONS) {
        sum += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        n++;
    }
    
    *iterations = n - 1;
    return sum;
}

int main(void) {
    double input_value;
    char buffer[100];
    
    printf("Enter a value for x (between %.10g and %g): ", MIN_INPUT, MAX_INPUT);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char* endptr;
    input_value = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < -MAX_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Input out of range\n");
        return EXIT_FAILURE;
    }
    
    if (fabs(input_value) < MIN_INPUT) {
        input_value = 0.0;
    }
    
    int iterations_used;
    double result = compute_series(input_value, &iterations_used);
    
    printf("sin(%.10g) approximation: %.15g\n", input_value, result);
    printf("math.h sin(%.10g): %.15g\n", input_value, sin(input_value));
    printf("Difference: %.15g\n", fabs(result - sin(input_value)));
    printf("Iterations used: %d\n", iterations_used);
    
    return EXIT_SUCCESS;
}