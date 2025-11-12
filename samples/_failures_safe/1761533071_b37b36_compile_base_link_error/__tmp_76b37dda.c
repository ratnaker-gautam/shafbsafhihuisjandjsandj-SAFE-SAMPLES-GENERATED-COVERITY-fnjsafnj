//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_mean(double* data, size_t n) {
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;
}

double calculate_stddev(double* data, size_t n, double mean) {
    double sum_sq = 0.0;
    for (size_t i = 0; i < n; i++) {
        double diff = data[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / n);
}

int parse_double(const char* str, double* result) {
    char* endptr;
    *result = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    size_t count = 0;
    
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
        
        double value;
        if (!parse_double(input, &value)) {
            printf("Invalid number: %s\n", input);
            continue;
        }
        
        if (value < -1e100 || value > 1e100) {
            printf("Number out of reasonable range: %s\n", input);
            continue;
        }
        
        numbers[count] = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    double mean = calculate_mean(numbers, count);
    double stddev = calculate_stddev(numbers, count, mean);
    
    printf("Statistics for %zu numbers:\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", stddev);
    printf("Minimum: %.6f\n", numbers[0]);
    printf("Maximum: %.6f\n", numbers[count - 1]);
    
    return 0;
}