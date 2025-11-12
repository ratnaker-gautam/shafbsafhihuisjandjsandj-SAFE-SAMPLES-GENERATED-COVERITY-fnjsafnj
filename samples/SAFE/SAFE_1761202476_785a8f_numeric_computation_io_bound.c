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
        if (i > INT_MAX / i) {
            fprintf(stderr, "Error: Term overflow in computation\n");
            exit(EXIT_FAILURE);
        }
        double term = 1.0 / ((double)i * (double)i);
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

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    long val = strtol(input, NULL, 10);
    if (val <= 0 || val > 1000000) return 0;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Series Sum Calculator (1 + 1/4 + 1/9 + ... + 1/n^2)\n");
    printf("Enter number of terms (1-1000000): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    } else if (len == sizeof(input) - 1) {
        fprintf(stderr, "Error: Input too long\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Error: Invalid input. Please enter a positive integer between 1 and 1000000\n");
        return EXIT_FAILURE;
    }
    
    int terms = (int)strtol(input, NULL, 10);
    if (terms <= 0 || terms > 1000000) {
        fprintf(stderr, "Error: Invalid term count\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series_sum(terms);
    double pi_squared_over_six = (3.14159265358979323846 * 3.14159265358979323846) / 6.0;
    
    printf("Sum of first %d terms: %.15f\n", terms, result);
    printf("Reference value (pi^2/6): %.15f\n", pi_squared_over_six);
    printf("Difference: %.15f\n", fabs(result - pi_squared_over_six));
    
    return EXIT_SUCCESS;
}