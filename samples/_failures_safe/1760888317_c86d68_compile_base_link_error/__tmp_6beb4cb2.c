//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

int is_valid_double(const char* str) {
    if (str == NULL || *str == '\0') return 0;
    
    int dot_count = 0;
    int digit_count = 0;
    
    if (*str == '-' || *str == '+') str++;
    
    while (*str != '\0') {
        if (*str == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (*str >= '0' && *str <= '9') {
            digit_count++;
        } else {
            return 0;
        }
        str++;
    }
    
    return digit_count > 0;
}

double safe_power(double base, double exponent) {
    if (base == 0.0 && exponent < 0.0) {
        return NAN;
    }
    
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) {
        return NAN;
    }
    
    double result = pow(base, exponent);
    
    if (isnan(result) || isinf(result)) {
        return NAN;
    }
    
    return result;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double base, exponent, result;
    
    printf("Scientific Calculator - Power Function\n");
    printf("Enter base: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid base value\n");
        return 1;
    }
    
    base = atof(input);
    
    printf("Enter exponent: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid exponent value\n");
        return 1;
    }
    
    exponent = atof(input);
    
    result = safe_power(base, exponent);
    
    if (isnan(result)) {
        printf("Error: Invalid mathematical operation\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    printf("\nComputing additional mathematical operations...\n");
    
    double sqrt_result = sqrt(fabs(base));
    double log_result = base > 0 ? log(base) : NAN;
    double sin_result = sin(base);
    
    printf("Square root of |base|: %.6f\n", sqrt_result);
    
    if (!isnan(log_result)) {
        printf("Natural logarithm of base: %.6f\n", log_result);
    } else {
        printf("Natural logarithm: undefined\n");
    }
    
    printf("Sine of base: %.6f\n", sin_result);
    
    return 0;
}