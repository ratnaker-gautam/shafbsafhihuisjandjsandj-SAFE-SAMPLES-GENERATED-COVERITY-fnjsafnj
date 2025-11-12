//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
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
        
        if (value == HUGE_VAL || value == -HUGE_VAL) {
            printf("Number out of range. Please enter a valid number.\n");
            continue;
        }
        
        *(ptr + count) = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
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
    double stddev = sqrt(variance);
    
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
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", stddev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    printf("\nSorted values:\n");
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (*(ptr + j) > *(ptr + j + 1)) {
                double temp = *(ptr + j);
                *(ptr + j) = *(ptr + j + 1);
                *(ptr + j + 1) = temp;
            }
        }
    }
    
    for (double *p = ptr; p < end; p++) {
        printf("%.6f ", *p);
    }
    printf("\n");
    
    return 0;
}