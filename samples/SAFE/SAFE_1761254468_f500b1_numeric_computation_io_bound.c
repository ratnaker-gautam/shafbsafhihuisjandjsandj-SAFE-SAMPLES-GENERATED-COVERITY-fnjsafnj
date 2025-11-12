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
        double term = 1.0 / (i * i);
        if (isinf(term) || isnan(term)) {
            break;
        }
        sum += term;
    }
    return sum;
}

double compute_pi_approximation(int iterations) {
    if (iterations <= 0) return 0.0;
    double pi_approx = 0.0;
    int sign = 1;
    for (int i = 0; i < iterations; i++) {
        double term = sign / (2.0 * i + 1.0);
        if (isinf(term) || isnan(term)) {
            break;
        }
        pi_approx += term;
        sign = -sign;
    }
    return 4.0 * pi_approx;
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
    int choice;
    
    printf("Numeric Computation Demo\n");
    printf("1. Compute series sum (1 + 1/4 + 1/9 + ...)\n");
    printf("2. Compute pi approximation (Leibniz formula)\n");
    printf("Enter choice (1 or 2): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!safe_str_to_int(input, &choice)) {
        fprintf(stderr, "Invalid choice format\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        fprintf(stderr, "Choice must be 1 or 2\n");
        return 1;
    }
    
    printf("Enter number of iterations/terms: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    int n;
    if (!safe_str_to_int(input, &n)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (n <= 0) {
        fprintf(stderr, "Number must be positive\n");
        return 1;
    }
    
    if (n > 1000000) {
        fprintf(stderr, "Number too large\n");
        return 1;
    }
    
    double result;
    if (choice == 1) {
        result = compute_series_sum(n);
        printf("Series sum with %d terms: %.10f\n", n, result);
    } else {
        result = compute_pi_approximation(n);
        printf("Pi approximation with %d iterations: %.10f\n", n, result);
    }
    
    return 0;
}