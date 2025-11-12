//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
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
    
    printf("Enter numbers (one per line, empty line to finish):\n");
    
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
        
        if (num == 0.0) {
            printf("Cannot process zero (division will fail)\n");
            continue;
        }
        
        numbers[count] = num;
        count++;
    }
    
    if (count < 2) {
        printf("Need at least 2 numbers for computation\n");
        return 1;
    }
    
    printf("\nComputing statistics for %d numbers:\n", count);
    
    double sum = 0.0;
    double product = 1.0;
    double min_val = numbers[0];
    double max_val = numbers[0];
    
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
        product *= numbers[i];
        if (numbers[i] < min_val) min_val = numbers[i];
        if (numbers[i] > max_val) max_val = numbers[i];
    }
    
    double mean = sum / count;
    
    double variance = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance += diff * diff;
    }
    variance /= count;
    double stddev = sqrt(variance);
    
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", stddev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    printf("\nComputing pairwise ratios:\n");
    int pair_count = 0;
    for (int i = 0; i < count && pair_count < 10; i++) {
        for (int j = i + 1; j < count && pair_count < 10; j++) {
            if (numbers[j] != 0.0) {
                double ratio = numbers[i] / numbers[j];
                printf("Ratio %d/%d: %.6f\n", i+1, j+1, ratio);
                pair_count++;
            }
        }
    }
    
    printf("\nComputing cumulative values:\n");
    double cumulative = 0.0;
    for (int i = 0; i < count; i++) {
        cumulative += numbers[i];
        printf("After number %d: %.6f\n", i+1, cumulative);
    }
    
    return 0;
}