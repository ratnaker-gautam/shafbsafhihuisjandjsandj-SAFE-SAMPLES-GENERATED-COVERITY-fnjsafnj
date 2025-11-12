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
    
    int decimal_points = 0;
    int digits = 0;
    
    if (*str == '-' || *str == '+') str++;
    
    while (*str) {
        if (*str >= '0' && *str <= '9') {
            digits++;
        } else if (*str == '.') {
            decimal_points++;
            if (decimal_points > 1) return 0;
        } else {
            return 0;
        }
        str++;
    }
    
    return digits > 0;
}

double safe_strtod(const char* str) {
    char* endptr;
    double val = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0') {
        return NAN;
    }
    
    if (val == HUGE_VAL || val == -HUGE_VAL) {
        return NAN;
    }
    
    return val;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    
    printf("Enter up to 100 numbers (one per line, empty line to finish):\n");
    
    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        if (!is_valid_double(input)) {
            printf("Invalid number format. Please enter a valid number.\n");
            continue;
        }
        
        double num = safe_strtod(input);
        if (isnan(num)) {
            printf("Number out of range. Please enter a valid number.\n");
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }
    
    printf("\nProcessing %d numbers...\n", count);
    
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
    
    printf("Results:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    printf("Standard Deviation: %.6f\n", std_dev);
    
    printf("\nComputing additional statistics...\n");
    
    int positive_count = 0;
    int negative_count = 0;
    int zero_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (numbers[i] > 0) positive_count++;
        else if (numbers[i] < 0) negative_count++;
        else zero_count++;
    }
    
    printf("Positive numbers: %d\n", positive_count);
    printf("Negative numbers: %d\n", negative_count);
    printf("Zero values: %d\n", zero_count);
    
    double magnitude_sum = 0.0;
    for (int i = 0; i < count; i++) {
        magnitude_sum += fabs(numbers[i]);
    }
    printf("Sum of absolute values: %.6f\n", magnitude_sum);
    
    return 0;
}