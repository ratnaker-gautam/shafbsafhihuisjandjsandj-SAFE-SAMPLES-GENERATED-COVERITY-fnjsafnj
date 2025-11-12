//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

double compute_series_sum(int terms) {
    if (terms <= 0) return 0.0;
    
    double sum = 0.0;
    for (int i = 1; i <= terms; i++) {
        if (i == 0) continue;
        double denominator = (double)i * (double)i;
        if (denominator == 0.0) continue;
        double term = 1.0 / denominator;
        if (isinf(term) || isnan(term)) {
            break;
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            sum = 0.0;
            break;
        }
    }
    return sum;
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    
    int has_digit = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (i == 0 && (input[i] == '-' || input[i] == '+')) {
            continue;
        }
        if (input[i] < '0' || input[i] > '9') {
            return 0;
        }
        has_digit = 1;
    }
    return has_digit;
}

int main(void) {
    char input_buffer[256];
    int terms;
    
    printf("Enter number of terms to compute series sum (1-%d): ", MAX_ITERATIONS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = 0;
    while (input_buffer[len] != '\0' && input_buffer[len] != '\n') {
        len++;
    }
    input_buffer[len] = '\0';
    
    if (!validate_input(input_buffer)) {
        fprintf(stderr, "Invalid input: must be a valid integer\n");
        return 1;
    }
    
    char* endptr;
    long val = strtol(input_buffer, &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid input: contains non-numeric characters\n");
        return 1;
    }
    
    if (val < 1 || val > MAX_ITERATIONS) {
        fprintf(stderr, "Input out of range: must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    if (val > INT_MAX) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    terms = (int)val;
    
    printf("Computing series sum for %d terms...\n", terms);
    
    double result = compute_series_sum(terms);
    
    printf("Result: %.15f\n", result);
    double pi = 3.14159265358979323846;
    printf("Expected limit (pi^2/6): %.15f\n", pi * pi / 6.0);
    
    return 0;
}