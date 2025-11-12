//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_series_sum(double x, int terms) {
    double sum = 0.0;
    double term = 1.0;
    
    for (int i = 1; i <= terms; i++) {
        term *= x / i;
        sum += term;
    }
    
    return sum + 1.0;
}

int validate_double_input(const char* input, double* result) {
    char* endptr;
    *result = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        return 0;
    }
    
    if (*result == HUGE_VAL || *result == -HUGE_VAL) {
        return 0;
    }
    
    return 1;
}

int validate_int_input(const char* input, int* result) {
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        return 0;
    }
    
    if (val < 1 || val > 1000) {
        return 0;
    }
    
    *result = (int)val;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double x;
    int terms;
    
    printf("Taylor series approximation of e^x\n");
    printf("Enter value for x: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!validate_double_input(input, &x)) {
        fprintf(stderr, "Invalid input for x\n");
        return 1;
    }
    
    printf("Enter number of terms (1-1000): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!validate_int_input(input, &terms)) {
        fprintf(stderr, "Invalid number of terms\n");
        return 1;
    }
    
    double result = compute_series_sum(x, terms);
    double reference = exp(x);
    double error = fabs(result - reference);
    
    printf("Approximation: %.15f\n", result);
    printf("Reference (exp(x)): %.15f\n", reference);
    printf("Absolute error: %.15f\n", error);
    printf("Relative error: %.15f%%\n", (error / fabs(reference)) * 100.0);
    
    return 0;
}