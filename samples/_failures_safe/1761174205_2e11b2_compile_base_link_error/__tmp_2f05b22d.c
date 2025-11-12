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

double safe_strtod(const char* str) {
    if (!is_valid_double(str)) {
        fprintf(stderr, "Error: Invalid numeric input\n");
        exit(EXIT_FAILURE);
    }
    
    char* endptr;
    double result = strtod(str, &endptr);
    
    if (*endptr != '\0') {
        fprintf(stderr, "Error: Invalid numeric input\n");
        exit(EXIT_FAILURE);
    }
    
    if (result == HUGE_VAL || result == -HUGE_VAL) {
        fprintf(stderr, "Error: Numeric overflow\n");
        exit(EXIT_FAILURE);
    }
    
    return result;
}

int main() {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    
    printf("Enter numbers (one per line, empty line to finish):\n");
    
    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (input[0] == '\0') {
            break;
        }
        
        if (len >= MAX_INPUT_LEN - 1) {
            fprintf(stderr, "Error: Input too long\n");
            return EXIT_FAILURE;
        }
        
        numbers[count] = safe_strtod(input);
        count++;
    }
    
    if (count == 0) {
        printf("No numbers entered.\n");
        return EXIT_SUCCESS;
    }
    
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
    double stddev = sqrt(variance);
    
    printf("Statistics for %d numbers:\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", stddev);
    
    return EXIT_SUCCESS;
}