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

double safe_power(double base, double exponent) {
    if (base == 0.0 && exponent < 0.0) return NAN;
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) return NAN;
    
    double result = pow(base, exponent);
    if (isnan(result) || isinf(result)) return NAN;
    return result;
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    double numbers[100];
    int count = 0;
    
    printf("Enter up to 100 numbers (one per line, empty line to finish):\n");
    
    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        if (!is_valid_double(input)) {
            printf("Invalid number format. Please enter a valid number.\n");
            continue;
        }
        
        char* endptr;
        double num = strtod(input, &endptr);
        if (*endptr != '\0') {
            printf("Invalid number format. Please enter a valid number.\n");
            continue;
        }
        
        if (isnan(num) || isinf(num)) {
            printf("Number out of representable range. Please enter a valid number.\n");
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }
    
    printf("\nComputing statistics for %d numbers:\n", count);
    
    double sum = 0.0;
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
        if (numbers[i] < min_val) min_val = numbers[i];
        if (numbers[i] > max_val) max_val = numbers[i];
    }
    
    double mean = sum / count;
    
    double variance_sum = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    
    printf("\nComputing geometric mean:\n");
    double log_sum = 0.0;
    int valid_geo = 1;
    for (int i = 0; i < count; i++) {
        if (numbers[i] <= 0.0) {
            valid_geo = 0;
            break;
        }
        log_sum += log(numbers[i]);
    }
    
    if (valid_geo) {
        double geometric_mean = exp(log_sum / count);
        printf("Geometric Mean: %.6f\n", geometric_mean);
    } else {
        printf("Geometric Mean: Cannot compute (non-positive values present)\n");
    }
    
    printf("\nComputing powers:\n");
    printf("Enter exponent for power computation: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading exponent.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_double(input)) {
        printf("Invalid exponent format.\n");
        return 1;
    }
    
    char* endptr;
    double exponent = strtod(input, &endptr);
    if (*endptr != '\0' || isnan(exponent) || isinf(exponent)) {
        printf("Invalid exponent value.\n");
        return 1;
    }
    
    printf("\nComputing each number raised to power %.6f:\n", exponent);
    for (int i = 0; i < count; i++) {
        double result = safe_power(numbers[i], exponent);
        if (