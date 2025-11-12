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
        n++;
        term = term * x / n;
        
        if (fabs(result) > 1e100) break;
    }
    
    return result;
}

double validate_input(const char* str) {
    if (!str || !str[0]) return -1.0;
    
    char* endptr;
    double val = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0') return -1.0;
    if (val <= 0.0 || val > MAX_INPUT) return -1.0;
    if (val < MIN_INPUT) return -1.0;
    
    return val;
}

int main(void) {
    char input[256];
    double x;
    
    printf("Enter a positive number (1e-10 to 1e6): ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Input error\n");
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
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    double result = compute_series(x, MAX_ITERATIONS);
    if (result < 0.0) {
        fprintf(stderr, "Computation error\n");
        return 1;
    }
    
    double reference = exp(x) - 1.0;
    double error = fabs(result - reference);
    
    printf("Input: %.10g\n", x);
    printf("Series result: %.15g\n", result);
    printf("Reference (e^x - 1): %.15g\n", reference);
    printf("Absolute error: %.15g\n", error);
    
    return 0;
}