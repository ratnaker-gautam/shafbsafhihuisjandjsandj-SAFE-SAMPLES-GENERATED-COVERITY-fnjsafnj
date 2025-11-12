//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series(double x, int max_iter) {
    if (x <= 0.0 || x > MAX_INPUT) return -1.0;
    
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        result += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        n++;
        
        if (fabs(result) > 1e100) break;
    }
    
    return result;
}

double validate_input(const char* input) {
    char* endptr;
    double value = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') return -1.0;
    if (value <= 0.0 || value > MAX_INPUT) return -1.0;
    if (value < MIN_INPUT) return -1.0;
    
    return value;
}

int main(void) {
    char input[256];
    double x;
    double result;
    
    printf("Enter a positive number (1e-10 to 1e6): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = 0;
    while (len < sizeof(input) && input[len] != '\0') {
        if (input[len] == '\n') {
            input[len] = '\0';
            break;
        }
        len++;
    }
    
    x = validate_input(input);
    if (x < 0.0) {
        fprintf(stderr, "Invalid input: must be positive number between 1e-10 and 1e6\n");
        return 1;
    }
    
    result = compute_series(x, MAX_ITERATIONS);
    
    if (result < 0.0) {
        fprintf(stderr, "Computation error: input out of valid range\n");
        return 1;
    }
    
    printf("Series result: %.10f\n", result);
    printf("Reference sin(%.10f): %.10f\n", x, sin(x));
    printf("Difference: %.10f\n", fabs(result - sin(x)));
    
    return 0;
}