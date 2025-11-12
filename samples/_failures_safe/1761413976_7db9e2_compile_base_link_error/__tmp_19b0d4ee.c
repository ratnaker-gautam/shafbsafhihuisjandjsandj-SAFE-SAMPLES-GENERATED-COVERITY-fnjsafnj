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
        
        *(data + count) = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    double *ptr = data;
    double sum = 0.0;
    double *end = data + count;
    
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
    double *min_ptr = data;
    double *max_ptr = data;
    
    while (ptr < end) {
        if (*ptr < *min_ptr) {
            min_ptr = ptr;
        }
        if (*ptr > *max_ptr) {
            max_ptr = ptr;
        }
        ptr++;
    }
    
    printf("\nResults:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", stddev);
    printf("Minimum: %.6f\n", *min_ptr);
    printf("Maximum: %.6f\n", *max_ptr);
    
    ptr = data;
    printf("\nSorted values:\n");
    for (int i = 0; i < count - 1; i++) {
        double *min = data + i;
        double *current = data + i + 1;
        while (current < end) {
            if (*current < *min) {
                min = current;
            }
            current++;
        }
        if (min != data + i) {
            double temp = *(data + i);
            *(data + i) = *min;
            *min = temp;
        }
    }
    
    ptr = data;
    while (ptr < end) {
        printf("%.6f ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}