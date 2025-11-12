//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int main() {
    double data[MAX_SIZE];
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
            printf("Invalid input. Enter a number or 'done': ");
            continue;
        }
        
        if (value == HUGE_VAL || value == -HUGE_VAL) {
            printf("Number out of range. Try again: ");
            continue;
        }
        
        *(data + count) = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 0;
    }
    
    double *start = data;
    double *end = data + count - 1;
    
    double sum = 0.0;
    double *ptr = start;
    while (ptr <= end) {
        sum += *ptr;
        ptr++;
    }
    
    double mean = sum / count;
    
    double variance = 0.0;
    ptr = start;
    while (ptr <= end) {
        double diff = *ptr - mean;
        variance += diff * diff;
        ptr++;
    }
    variance /= count;
    
    double stddev = sqrt(variance);
    
    double min_val = *start;
    double max_val = *start;
    ptr = start + 1;
    while (ptr <= end) {
        if (*ptr < min_val) {
            min_val = *ptr;
        }
        if (*ptr > max_val) {
            max_val = *ptr;
        }
        ptr++;
    }
    
    printf("\nResults:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", stddev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    return 0;
}