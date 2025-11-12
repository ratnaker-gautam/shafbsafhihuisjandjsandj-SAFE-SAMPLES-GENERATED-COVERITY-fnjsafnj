//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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

double safe_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0.0) return 0.0;
    
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > DBL_MAX / fabs(base)) {
            return exponent > 0 ? DBL_MAX : 0.0;
        }
        result *= base;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}

int main() {
    char input[MAX_INPUT_LEN];
    double base;
    int exponent;
    
    printf("Enter base (double): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid base format\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &base) != 1) {
        fprintf(stderr, "Error parsing base\n");
        return 1;
    }
    
    printf("Enter exponent (integer): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    char* endptr;
    long exp_long = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') {
        fprintf(stderr, "Invalid exponent format\n");
        return 1;
    }
    
    if (exp_long < INT_MIN || exp_long > INT_MAX) {
        fprintf(stderr, "Exponent out of range\n");
        return 1;
    }
    exponent = (int)exp_long;
    
    if (base == 0.0 && exponent < 0) {
        fprintf(stderr, "Cannot raise 0 to negative power\n");
        return 1;
    }
    
    double result = safe_power(base, exponent);
    
    if (result == DBL_MAX && exponent > 0 && base != 0.0) {
        fprintf(stderr, "Result overflow\n");
        return 1;
    }
    
    if (result == 0.0 && exponent > 0 && base != 0.0) {
        fprintf(stderr, "Result underflow\n");
        return 1;
    }
    
    printf("Result: %.15g\n", result);
    
    return 0;
}