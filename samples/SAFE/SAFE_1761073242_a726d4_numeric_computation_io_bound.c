//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_series_sum(int n_terms) {
    double sum = 0.0;
    for (int i = 1; i <= n_terms; i++) {
        if (i == 0) {
            return -1.0;
        }
        double denominator = (double)i * (double)i;
        if (denominator == 0.0) {
            return -1.0;
        }
        double term = 1.0 / denominator;
        if (isinf(term) || isnan(term)) {
            return -1.0;
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            return -1.0;
        }
    }
    return sum;
}

int validate_integer_input(const char* input, int* result) {
    if (input == NULL || result == NULL) {
        return 0;
    }
    
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    
    if (val < 1 || val > 1000000) {
        return 0;
    }
    
    if (val > INT_MAX) {
        return 0;
    }
    
    *result = (int)val;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    int n_terms;
    
    printf("Enter number of terms for series computation (1-1000000): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    } else if (len == MAX_INPUT_LEN) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!validate_integer_input(input_buffer, &n_terms)) {
        fprintf(stderr, "Invalid input. Please enter an integer between 1 and 1000000\n");
        return 1;
    }
    
    double result = compute_series_sum(n_terms);
    if (result < 0) {
        fprintf(stderr, "Numerical error in computation\n");
        return 1;
    }
    
    printf("Sum of first %d terms: %.15f\n", n_terms, result);
    printf("Theoretical limit (π²/6): %.15f\n", 3.14159265358979323846 * 3.14159265358979323846 / 6.0);
    
    return 0;
}