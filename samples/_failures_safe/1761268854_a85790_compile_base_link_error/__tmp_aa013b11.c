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
    double *current = data;
    for (int i = 0; i < count; i++) {
        double diff = *(current + i) - mean;
        sum_sq_diff += diff * diff;
    }
    
    variance = sum_sq_diff / count;
    std_dev = sqrt(variance);
    
    printf("Statistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    
    double *min_ptr = data;
    double *max_ptr = data;
    
    for (int i = 1; i < count; i++) {
        if (*(data + i) < *min_ptr) {
            min_ptr = data + i;
        }
        if (*(data + i) > *max_ptr) {
            max_ptr = data + i;
        }
    }
    
    printf("Minimum: %.6f\n", *min_ptr);
    printf("Maximum: %.6f\n", *max_ptr);
    
    double median;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (*(data + j) > *(data + j + 1)) {
                double temp = *(data + j);
                *(data + j) = *(data + j + 1);
                *(data + j + 1) = temp;
            }
        }
    }
    
    if (count % 2 == 0) {
        median = (*(data + count/2 - 1) + *(data + count/2)) / 2.0;
    } else {
        median = *(data + count/2);
    }
    
    printf("Median: %.6f\n", median);
    
    return 0;
}