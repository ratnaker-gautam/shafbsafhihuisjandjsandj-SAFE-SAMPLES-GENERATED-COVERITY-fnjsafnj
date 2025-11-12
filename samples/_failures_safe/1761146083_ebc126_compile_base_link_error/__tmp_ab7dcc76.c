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
    if (base == 0.0 && exponent <= 0.0) return NAN;
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) return NAN;
    
    double result = pow(base, exponent);
    if (isnan(result) || isinf(result)) return NAN;
    return result;
}

int main() {
    char input[MAX_INPUT_LEN];
    double base, exponent, result;
    
    printf("Numeric Computation: Power Calculator\n");
    printf("Enter base: ");
    
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
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid base value\n");
        return 1;
    }
    
    base = strtod(input, NULL);
    
    printf("Enter exponent: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    } else if (len == sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid exponent value\n");
        return 1;
    }
    
    exponent = strtod(input, NULL);
    
    result = safe_power(base, exponent);
    
    if (isnan(result)) {
        printf("Error: Invalid mathematical operation\n");
        return 1;
    }
    
    printf("Result: %.6f ^ %.6f = %.6f\n", base, exponent, result);
    
    printf("\nAdditional computations:\n");
    printf("Square root of result: %.6f\n", sqrt(fabs(result)));
    printf("Natural log of |result|: %.6f\n", log(fabs(result)));
    printf("Result rounded: %.0f\n", round(result));
    printf("Result ceiling: %.0f\n", ceil(result));
    printf("Result floor: %.0f\n", floor(result));
    
    return 0;
}