//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_series_sum(int n) {
    if (n <= 0) return 0.0;
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        double denominator = (double)i * (double)i;
        if (denominator == 0.0) continue;
        double term = 1.0 / denominator;
        if (isinf(term) || isnan(term)) {
            return sum;
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            return sum;
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
    char input[MAX_INPUT_LEN + 1] = {0};
    int n;
    
    printf("Enter number of terms for series sum (1 to 100000): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len == MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!safe_str_to_int(input, &n)) {
        fprintf(stderr, "Invalid integer input\n");
        return 1;
    }
    
    if (n < 1 || n > 100000) {
        fprintf(stderr, "Number must be between 1 and 100000\n");
        return 1;
    }
    
    double result = compute_series_sum(n);
    
    if (isinf(result) || isnan(result)) {
        fprintf(stderr, "Numerical error in computation\n");
        return 1;
    }
    
    printf("Sum of series with %d terms: %.15f\n", n, result);
    printf("Reference value (π²/6): %.15f\n", 3.14159265358979323846 * 3.14159265358979323846 / 6.0);
    
    return 0;
}