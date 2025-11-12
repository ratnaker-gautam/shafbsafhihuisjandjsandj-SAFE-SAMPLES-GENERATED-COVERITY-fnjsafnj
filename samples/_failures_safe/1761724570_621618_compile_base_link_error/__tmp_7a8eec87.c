//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 100

int validate_input(const char* input, int* valid) {
    if (input == NULL || valid == NULL) return -1;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return -1;
    if (value < 1 || value > MAX_SIZE) return -1;
    if (value > INT_MAX) return -1;
    
    *valid = (int)value;
    return 0;
}

void compute_statistics(int* data, size_t count, double* results) {
    if (data == NULL || results == NULL || count == 0) return;
    
    int* ptr = data;
    double sum = 0.0;
    double min = (double)*ptr;
    double max = (double)*ptr;
    
    for (size_t i = 0; i < count; i++) {
        double current = (double)*(ptr + i);
        sum += current;
        if (current < min) min = current;
        if (current > max) max = current;
    }
    
    double mean = sum / (double)count;
    double variance = 0.0;
    
    for (size_t i = 0; i < count; i++) {
        double diff = (double)*(data + i) - mean;
        variance += diff * diff;
    }
    variance /= (double)count;
    
    results[0] = mean;
    results[1] = sqrt(variance);
    results[2] = min;
    results[3] = max;
}

int main() {
    char buffer[256];
    int size;
    
    printf("Enter number of elements (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    if (validate_input(buffer, &size) != 0) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    int data[MAX_SIZE];
    printf("Enter %d integers:\n", size);
    
    for (int i = 0; i < size; i++) {
        printf("Element %d: ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        int value;
        if (validate_input(buffer, &value) != 0) {
            fprintf(stderr, "Invalid input value\n");
            return 1;
        }
        
        *(data + i) = value;
    }
    
    double results[4];
    compute_statistics(data, size, results);
    
    printf("\nStatistical Results:\n");
    printf("Mean: %.2f\n", results[0]);
    printf("Standard Deviation: %.2f\n", results[1]);
    printf("Minimum: %.2f\n", results[2]);
    printf("Maximum: %.2f\n", results[3]);
    
    return 0;
}