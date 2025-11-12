//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int validate_input(const char* input, int* valid) {
    if (input == NULL || valid == NULL) return -1;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return -1;
    if (value < 1 || value > MAX_SIZE) return -1;
    
    *valid = (int)value;
    return 0;
}

void compute_statistics(int* data, size_t size, double* results) {
    if (data == NULL || results == NULL || size == 0) return;
    
    int* ptr = data;
    int* end = data + size;
    
    double sum = 0.0;
    int min = *ptr;
    int max = *ptr;
    
    while (ptr < end) {
        sum += *ptr;
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        ptr++;
    }
    
    double mean = sum / size;
    
    ptr = data;
    double variance = 0.0;
    while (ptr < end) {
        double diff = *ptr - mean;
        variance += diff * diff;
        ptr++;
    }
    variance /= size;
    
    results[0] = mean;
    results[1] = sqrt(variance);
    results[2] = (double)min;
    results[3] = (double)max;
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
    int* ptr = data;
    
    printf("Enter %d integers:\n", size);
    for (int i = 0; i < size; i++) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        char* endptr;
        long value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\0') {
            fprintf(stderr, "Invalid number format\n");
            return 1;
        }
        
        if (value < -1000000 || value > 1000000) {
            fprintf(stderr, "Number out of range\n");
            return 1;
        }
        
        *ptr = (int)value;
        ptr++;
    }
    
    double results[4];
    compute_statistics(data, size, results);
    
    printf("\nStatistics:\n");
    printf("Mean: %.2f\n", results[0]);
    printf("Standard Deviation: %.2f\n", results[1]);
    printf("Minimum: %.0f\n", results[2]);
    printf("Maximum: %.0f\n", results[3]);
    
    return 0;
}