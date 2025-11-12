//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_series_sum(int terms) {
    double sum = 0.0;
    for (int i = 1; i <= terms; i++) {
        if (i == 0) {
            return sum;
        }
        double denominator = (double)i * (double)i;
        if (isinf(denominator) || isnan(denominator)) {
            return sum;
        }
        double term = 1.0 / denominator;
        if (isinf(term) || isnan(term)) {
            return sum;
        }
        double new_sum = sum + term;
        if (isinf(new_sum) || isnan(new_sum)) {
            return sum;
        }
        sum = new_sum;
    }
    return sum;
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char buffer[MAX_INPUT_LEN];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        int valid = 1;
        for (size_t i = 0; i < len; i++) {
            if (i == 0 && buffer[i] == '-') continue;
            if (buffer[i] < '0' || buffer[i] > '9') {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        char* endptr;
        long long_val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\0') {
            printf("Invalid number format.\n");
            continue;
        }
        
        if (long_val < min_val || long_val > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        value = (int)long_val;
        if (value != long_val) {
            printf("Value out of integer range.\n");
            continue;
        }
        
        break;
    }
    
    return value;
}

int main(void) {
    printf("Series Sum Calculator\n");
    printf("====================\n\n");
    
    int terms = get_valid_int("Enter number of terms (1-1000000): ", 1, 1000000);
    
    printf("\nComputing sum of 1/n^2 for n=1 to %d...\n", terms);
    
    double result = compute_series_sum(terms);
    double pi_approx = sqrt(result * 6.0);
    double actual_pi = 3.14159265358979323846;
    
    printf("Series sum: %.15f\n", result);
    printf("Approximation of pi: %.15f\n", pi_approx);
    printf("Actual pi: %.15f\n", actual_pi);
    printf("Difference: %.15f\n", fabs(pi_approx - actual_pi));
    
    return 0;
}