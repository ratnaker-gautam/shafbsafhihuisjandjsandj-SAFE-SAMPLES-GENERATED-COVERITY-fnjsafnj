//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int main() {
    double data[MAX_SIZE];
    double *ptr = data;
    int count = 0;
    char buffer[256];
    
    printf("Enter numbers (max %d), one per line. Enter 'done' to finish:\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (strncmp(buffer, "done", 4) == 0) {
            break;
        }
        
        char *endptr;
        double value = strtod(buffer, &endptr);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a number or 'done'.\n");
            continue;
        }
        
        if (value == HUGE_VAL || value == -HUGE_VAL) {
            printf("Number out of range. Please enter a valid number.\n");
            continue;
        }
        
        *(ptr + count) = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 0;
    }
    
    double sum = 0.0;
    double *end = ptr + count;
    
    for (double *p = ptr; p < end; p++) {
        sum += *p;
    }
    
    double mean = sum / count;
    
    double variance_sum = 0.0;
    for (double *p = ptr; p < end; p++) {
        double diff = *p - mean;
        variance_sum += diff * diff;
    }
    
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    double min_val = *ptr;
    double max_val = *ptr;
    
    for (double *p = ptr + 1; p < end; p++) {
        if (*p < min_val) {
            min_val = *p;
        }
        if (*p > max_val) {
            max_val = *p;
        }
    }
    
    printf("\nStatistics for %d numbers:\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    return 0;
}