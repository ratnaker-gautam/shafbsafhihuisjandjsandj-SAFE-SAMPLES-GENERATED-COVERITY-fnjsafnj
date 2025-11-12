//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_mean(double* data, size_t n) {
    if (n == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;
}

double calculate_stddev(double* data, size_t n, double mean) {
    if (n <= 1) return 0.0;
    double sum_sq = 0.0;
    for (size_t i = 0; i < n; i++) {
        double diff = data[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (n - 1));
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
    double data[1000];
    size_t count = 0;
    
    printf("Enter up to 1000 numbers (one per line, empty line to finish):\n");
    
    while (count < 1000) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
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
        
        data[count] = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    double mean = calculate_mean(data, count);
    double stddev = calculate_stddev(data, count, mean);
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %zu\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", stddev);
    
    printf("\nData points more than 2 standard deviations from mean:\n");
    int outliers = 0;
    for (size_t i = 0; i < count; i++) {
        double z_score = fabs(data[i] - mean) / stddev;
        if (z_score > 2.0 && stddev > 0.0) {
            printf("Index %zu: %.6f (z-score: %.3f)\n", i, data[i], z_score);
            outliers++;
        }
    }
    
    if (outliers == 0) {
        printf("No outliers found.\n");
    }
    
    return 0;
}