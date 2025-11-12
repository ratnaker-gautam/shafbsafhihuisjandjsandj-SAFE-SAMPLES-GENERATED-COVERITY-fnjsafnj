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
        if (isinf(sum) || isnan(sum)) {
            sum = HUGE_VAL;
            break;
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
    
    printf("Enter number of terms for series sum (1 to 1000000): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    } else if (len == sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!safe_str_to_int(input, &terms)) {
        fprintf(stderr, "Invalid integer input\n");
        return 1;
    }
    
    if (terms < 1 || terms > 1000000) {
        fprintf(stderr, "Terms must be between 1 and 1000000\n");
        return 1;
    }
    
    double result = compute_series_sum(terms);
    
    if (isinf(result)) {
        printf("Result: Infinity (numerical overflow)\n");
    } else if (isnan(result)) {
        printf("Result: NaN (undefined)\n");
    } else {
        printf("Sum of %d terms: %.15f\n", terms, result);
        printf("Approximation to pi^2/6: %.15f\n", 3.14159265358979323846 * 3.14159265358979323846 / 6.0);
    }
    
    return 0;
}