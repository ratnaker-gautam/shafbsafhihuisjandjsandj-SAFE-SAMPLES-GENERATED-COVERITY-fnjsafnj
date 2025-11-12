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
    double *current = ptr;
    
    while (current < end) {
        sum += *current;
        current++;
    }
    
    double mean = sum / count;
    
    double variance_sum = 0.0;
    current = ptr;
    
    while (current < end) {
        double diff = *current - mean;
        variance_sum += diff * diff;
        current++;
    }
    
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    double min_val = *ptr;
    double max_val = *ptr;
    current = ptr + 1;
    
    while (current < end) {
        if (*current < min_val) {
            min_val = *current;
        }
        if (*current > max_val) {
            max_val = *current;
        }
        current++;
    }
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    return 0;
}