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
        
        if (value != value) {
            printf("Invalid number (NaN). Please enter a valid number.\n");
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
    double *sum_ptr = data;
    for (int i = 0; i < count; i++) {
        sum += *(sum_ptr + i);
    }
    
    double mean = sum / count;
    
    double variance = 0.0;
    double *var_ptr = data;
    for (int i = 0; i < count; i++) {
        double diff = *(var_ptr + i) - mean;
        variance += diff * diff;
    }
    variance /= count;
    
    double std_dev = sqrt(variance);
    
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
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", *min_ptr);
    printf("Maximum: %.6f\n", *max_ptr);
    
    return 0;
}