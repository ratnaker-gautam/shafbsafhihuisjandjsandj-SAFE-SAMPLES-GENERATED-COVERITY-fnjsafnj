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
        if (i == 0) continue;
        double denominator = i * i;
        if (denominator == 0.0) continue;
        double term = 1.0 / denominator;
        if (isinf(term) || isnan(term)) {
            break;
        }
        sum += term;
    }
    return sum;
}

double compute_pi_approximation(int iterations) {
    if (iterations <= 0) return 0.0;
    double sum = 0.0;
    int sign = 1;
    for (int i = 0; i < iterations; i++) {
        double denominator = 2.0 * i + 1.0;
        if (denominator == 0.0) continue;
        double term = sign / denominator;
        if (isinf(term) || isnan(term)) {
            break;
        }
        sum += term;
        sign = -sign;
    }
    return 4.0 * sum;
}

int safe_str_to_int(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    char* endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *result = (int)val;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    int choice, param;
    
    printf("Numeric Computation Demo\n");
    printf("1. Compute sum of 1/n^2 series\n");
    printf("2. Compute pi approximation using Leibniz formula\n");
    printf("Enter choice (1 or 2): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (!safe_str_to_int(input, &choice)) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        fprintf(stderr, "Choice must be 1 or 2\n");
        return 1;
    }
    
    printf("Enter number of terms/iterations (1-1000000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (!safe_str_to_int(input, &param)) {
        fprintf(stderr, "Invalid parameter\n");
        return 1;
    }
    
    if (param < 1 || param > 1000000) {
        fprintf(stderr, "Parameter out of range\n");
        return 1;
    }
    
    double result;
    const double pi = 3.14159265358979323846;
    const double pi_squared_over_six = 1.64493406684822640607;
    
    if (choice == 1) {
        result = compute_series_sum(param);
        printf("Sum of 1/n^2 series with %d terms: %.15f\n", param, result);
        printf("Expected limit (pi^2/6): %.15f\n", pi_squared_over_six);
    } else {
        result = compute_pi_approximation(param);
        printf("Pi approximation with %d iterations: %.15f\n", param, result);
        printf("Actual pi value: %.15f\n", pi);
    }
    
    double expected = (choice == 1) ? pi_squared_over_six : pi;
    double error = fabs(result - expected);
    printf("Absolute error: %.15f\n", error);
    
    return 0;
}