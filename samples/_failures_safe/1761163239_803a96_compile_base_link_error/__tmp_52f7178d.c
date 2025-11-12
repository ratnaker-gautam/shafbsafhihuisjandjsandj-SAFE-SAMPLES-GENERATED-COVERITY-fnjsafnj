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
    double mean, variance, std_dev;
    int count = 0;
    char buffer[256];
    
    printf("Enter up to %d numbers (empty line to finish):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (buffer[0] == '\n') {
            break;
        }
        
        char *endptr;
        double value = strtod(buffer, &endptr);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        if (value < -1e100 || value > 1e100) {
            printf("Number out of range.\n");
            continue;
        }
        
        *(ptr + count) = value;
        sum += value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    mean = sum / count;
    
    double sum_sq_diff = 0.0;
    double *current = data;
    for (int i = 0; i < count; i++) {
        double diff = *(current + i) - mean;
        sum_sq_diff += diff * diff;
    }
    
    variance = sum_sq_diff / count;
    std_dev = sqrt(variance);
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    
    printf("\nData with deviations from mean:\n");
    current = data;
    for (int i = 0; i < count; i++) {
        double deviation = *(current + i) - mean;
        printf("Value[%d]: %.6f, Deviation: %+.6f\n", i, *(current + i), deviation);
    }
    
    double min_val = *(data);
    double max_val = *(data);
    current = data + 1;
    for (int i = 1; i < count; i++) {
        if (*(current + i - 1) < min_val) {
            min_val = *(current + i - 1);
        }
        if (*(current + i - 1) > max_val) {
            max_val = *(current + i - 1);
        }
    }
    
    printf("\nRange: %.6f to %.6f\n", min_val, max_val);
    printf("Data range: %.6f\n", max_val - min_val);
    
    return 0;
}