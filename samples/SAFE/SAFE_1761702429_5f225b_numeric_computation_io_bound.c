//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_series_sum(int terms) {
    if (terms <= 0) return 0.0;
    double sum = 0.0;
    for (int i = 1; i <= terms; i++) {
        if (i == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            exit(EXIT_FAILURE);
        }
        double denominator = (double)i * (double)i;
        if (denominator == 0.0) {
            fprintf(stderr, "Error: Division by zero\n");
            exit(EXIT_FAILURE);
        }
        double term = 1.0 / denominator;
        if (isinf(term) || isnan(term)) {
            fprintf(stderr, "Error: Term overflow in computation\n");
            exit(EXIT_FAILURE);
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            fprintf(stderr, "Error: Sum overflow in computation\n");
            exit(EXIT_FAILURE);
        }
    }
    return sum;
}

int safe_str_to_int(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    
    char* endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (val > INT_MAX || val < INT_MIN) return 0;
    
    *result = (int)val;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    int terms;
    
    printf("Series Sum Calculator (1 + 1/4 + 1/9 + 1/16 + ...)\n");
    printf("Enter number of terms to compute: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len == sizeof(input) - 1) {
        fprintf(stderr, "Error: Input too long\n");
        return EXIT_FAILURE;
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (!safe_str_to_int(input, &terms)) {
        fprintf(stderr, "Error: Invalid integer input\n");
        return EXIT_FAILURE;
    }
    
    if (terms < 1) {
        fprintf(stderr, "Error: Number of terms must be positive\n");
        return EXIT_FAILURE;
    }
    
    if (terms > 1000000) {
        fprintf(stderr, "Error: Too many terms (max: 1000000)\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series_sum(terms);
    double pi_squared_over_six = (3.14159265358979323846 * 3.14159265358979323846) / 6.0;
    
    printf("Sum of first %d terms: %.15f\n", terms, result);
    printf("Approximation to pi^2/6: %.15f\n", pi_squared_over_six);
    printf("Difference: %.15f\n", fabs(result - pi_squared_over_six));
    
    return EXIT_SUCCESS;
}