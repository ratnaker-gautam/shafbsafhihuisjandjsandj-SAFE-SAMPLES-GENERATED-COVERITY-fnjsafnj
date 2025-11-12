//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int main(void) {
    double data[MAX_SIZE];
    double *ptr = data;
    double sum = 0.0;
    double mean, variance, stddev;
    int count = 0;
    char input[256];
    
    printf("Enter up to %d numbers (empty line to finish):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == '\n') {
            break;
        }
        
        char *endptr;
        double value = strtod(input, &endptr);
        
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        if (value == HUGE_VAL || value == -HUGE_VAL) {
            printf("Number out of range. Please enter a valid number.\n");
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
    for (int i = 0; i < count; i++) {
        double diff = *(ptr + i) - mean;
        sum_sq_diff += diff * diff;
    }
    
    variance = sum_sq_diff / count;
    stddev = sqrt(variance);
    
    printf("Statistics for %d numbers:\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", stddev);
    
    double *min_ptr = ptr;
    double *max_ptr = ptr;
    
    for (int i = 1; i < count; i++) {
        if (*(ptr + i) < *min_ptr) {
            min_ptr = ptr + i;
        }
        if (*(ptr + i) > *max_ptr) {
            max_ptr = ptr + i;
        }
    }
    
    printf("Minimum: %.6f\n", *min_ptr);
    printf("Maximum: %.6f\n", *max_ptr);
    
    return 0;
}