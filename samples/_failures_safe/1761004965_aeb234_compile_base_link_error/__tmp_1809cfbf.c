//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

int is_valid_double(const char* str, double* result) {
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
    int count = 0;
    
    printf("Enter up to 100 numbers (one per line, empty line to finish):\n");
    
    while (count < 100) {
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
        
        double num;
        if (!is_valid_double(input, &num)) {
            printf("Invalid number: %s\n", input);
            continue;
        }
        
        if (num == 0.0 && !(input[0] >= '0' && input[0] <= '9')) {
            printf("Invalid number: %s\n", input);
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 0;
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
    double std_dev = sqrt(variance);
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Standard Deviation: %.6f\n", std_dev);
    
    printf("\nComputing additional statistics...\n");
    
    double geometric_mean = 1.0;
    int valid_geometric = 1;
    for (int i = 0; i < count; i++) {
        if (numbers[i] <= 0.0) {
            valid_geometric = 0;
            break;
        }
        geometric_mean *= numbers[i];
    }
    
    if (valid_geometric) {
        geometric_mean = pow(geometric_mean, 1.0 / count);
        printf("Geometric Mean: %.6f\n", geometric_mean);
    } else {
        printf("Geometric Mean: Cannot compute (non-positive values)\n");
    }
    
    double harmonic_mean = 0.0;
    int valid_harmonic = 1;
    for (int i = 0; i < count; i++) {
        if (numbers[i] == 0.0) {
            valid_harmonic = 0;
            break;
        }
        harmonic_mean += 1.0 / numbers[i];
    }
    
    if (valid_harmonic) {
        harmonic_mean = count / harmonic_mean;
        printf("Harmonic Mean: %.6f\n", harmonic_mean);
    } else {
        printf("Harmonic Mean: Cannot compute (zero values)\n");
    }
    
    printf("\nNormalized values (z-scores):\n");
    for (int i = 0; i < count; i++) {
        double z_score = (numbers[i] - mean) / std_dev;
        printf("Value %.6f -> Z-score: %.6f\n", numbers[i], z_score);
    }
    
    return 0;
}