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
            printf("Number out of range. Please enter a smaller number.\n");
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
    
    while (ptr < end) {
        sum += *ptr;
        ptr++;
    }
    
    double mean = sum / count;
    
    ptr = data;
    double variance = 0.0;
    
    while (ptr < end) {
        double diff = *ptr - mean;
        variance += diff * diff;
        ptr++;
    }
    
    variance /= count;
    double stddev = sqrt(variance);
    
    ptr = data;
    double min_val = *ptr;
    double max_val = *ptr;
    
    while (ptr < end) {
        if (*ptr < min_val) {
            min_val = *ptr;
        }
        if (*ptr > max_val) {
            max_val = *ptr;
        }
        ptr++;
    }
    
    printf("Statistics for %d numbers:\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", stddev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    return 0;
}