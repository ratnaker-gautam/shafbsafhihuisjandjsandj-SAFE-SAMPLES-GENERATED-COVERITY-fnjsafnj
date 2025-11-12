//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
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
    
    while (*str) {
        if (*str >= '0' && *str <= '9') {
            digit_count++;
        } else if (*str == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else {
            return 0;
        }
        str++;
    }
    
    return digit_count > 0;
}

double safe_pow(double base, double exponent) {
    if (base == 0.0 && exponent < 0.0) return NAN;
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) return NAN;
    
    double result = pow(base, exponent);
    if (!isfinite(result)) return NAN;
    return result;
}

int main() {
    char input[MAX_INPUT_LEN];
    double base, exponent, result;
    
    printf("Numerical Power Calculator\n");
    printf("Enter base: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
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
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid exponent value\n");
        return 1;
    }
    
    exponent = atof(input);
    
    result = safe_pow(base, exponent);
    
    if (isnan(result)) {
        printf("Error: Invalid mathematical operation\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    printf("Verification steps:\n");
    printf("Base: %.6f\n", base);
    printf("Exponent: %.6f\n", exponent);
    
    if (exponent == 0.0) {
        printf("Any number to the power of 0 is 1\n");
    } else if (exponent == 1.0) {
        printf("Any number to the power of 1 is itself\n");
    } else if (base == 0.0) {
        printf("0 to any positive power is 0\n");
    } else if (base == 1.0) {
        printf("1 to any power is 1\n");
    }
    
    return 0;
}