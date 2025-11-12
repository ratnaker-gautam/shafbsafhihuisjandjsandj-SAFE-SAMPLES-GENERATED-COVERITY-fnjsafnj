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
    
    printf("Enter up to %d numbers (one per line, empty line to finish):\n", MAX_SIZE);
    
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
        
        *(data + count) = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
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
    
    double variance_sum = 0.0;
    ptr = start;
    while (ptr <= end) {
        double diff = *ptr - mean;
        variance_sum += diff * diff;
        ptr++;
    }
    
    double variance = variance_sum / count;
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
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %d\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", stddev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    printf("\nSorted values (ascending):\n");
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            double *current = data + j;
            double *next = data + j + 1;
            if (*current > *next) {
                double temp = *current;
                *current = *next;
                *next = temp;
            }
        }
    }
    
    ptr = start;
    int printed = 0;
    while (ptr <= end) {
        printf("%.6f", *ptr);
        printed++;
        if (printed % 5 == 0) {
            printf("\n");
        } else if (ptr < end) {
            printf(", ");
        }
        ptr++;
    }
    if (printed % 5 != 0) {
        printf("\n");
    }
    
    return 0;
}