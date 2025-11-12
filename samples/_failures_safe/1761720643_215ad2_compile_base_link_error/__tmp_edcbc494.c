//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 100

int main(void) {
    double data[MAX_SIZE];
    double *ptr = data;
    double sum = 0.0;
    double mean = 0.0;
    double variance = 0.0;
    double stddev = 0.0;
    int count = 0;
    char input[256];
    char *endptr;
    
    printf("Enter up to %d numbers (empty line to finish):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == '\n' || input[0] == '\0') {
            break;
        }
        
        double value = strtod(input, &endptr);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        if (value < -1e100 || value > 1e100) {
            printf("Number out of reasonable range.\n");
            continue;
        }
        
        *ptr = value;
        sum += value;
        ptr++;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    mean = sum / count;
    
    ptr = data;
    for (int i = 0; i < count; i++) {
        double diff = *ptr - mean;
        variance += diff * diff;
        ptr++;
    }
    variance /= count;
    stddev = sqrt(variance);
    
    ptr = data;
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", stddev);
    
    printf("\nNormalized values (z-scores):\n");
    for (int i = 0; i < count; i++) {
        double zscore = (*ptr - mean) / stddev;
        printf("Value %d: %.6f (z-score: %.6f)\n", i + 1, *ptr, zscore);
        ptr++;
    }
    
    ptr = data;
    double min_val = *ptr;
    double max_val = *ptr;
    for (int i = 1; i < count; i++) {
        ptr++;
        if (*ptr < min_val) min_val = *ptr;
        if (*ptr > max_val) max_val = *ptr;
    }
    
    printf("\nRange: %.6f to %.6f\n", min_val, max_val);
    printf("Data span: %.6f\n", max_val - min_val);
    
    return 0;
}