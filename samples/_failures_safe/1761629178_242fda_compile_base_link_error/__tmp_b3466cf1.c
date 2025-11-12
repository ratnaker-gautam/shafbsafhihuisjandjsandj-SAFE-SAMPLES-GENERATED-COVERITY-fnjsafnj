//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 100

int validate_input(const char* input, int* valid) {
    if (input == NULL || valid == NULL) return -1;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return -1;
    if (value < INT_MIN || value > INT_MAX) return -1;
    
    *valid = (int)value;
    return 0;
}

void compute_statistics(int* data, size_t count, double* results) {
    if (data == NULL || results == NULL || count == 0) return;
    
    double sum = 0.0;
    double min = (double)INT_MAX;
    double max = (double)INT_MIN;
    
    int* ptr = data;
    for (size_t i = 0; i < count; i++) {
        double current = (double)(*ptr);
        sum += current;
        if (current < min) min = current;
        if (current > max) max = current;
        ptr++;
    }
    
    double mean = sum / (double)count;
    
    double variance = 0.0;
    ptr = data;
    for (size_t i = 0; i < count; i++) {
        double diff = (double)(*ptr) - mean;
        variance += diff * diff;
        ptr++;
    }
    variance /= (double)count;
    
    results[0] = mean;
    results[1] = sqrt(variance);
    results[2] = min;
    results[3] = max;
}

int main() {
    int data[ARRAY_SIZE];
    char input_buffer[64];
    
    printf("Enter number of elements (1-%d): ", ARRAY_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    int count;
    if (validate_input(input_buffer, &count) != 0 || count <= 0 || count > ARRAY_SIZE) {
        fprintf(stderr, "Invalid count\n");
        return 1;
    }
    
    printf("Enter %d integers:\n", count);
    for (int i = 0; i < count; i++) {
        printf("Element %d: ", i + 1);
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        int value;
        if (validate_input(input_buffer, &value) != 0) {
            fprintf(stderr, "Invalid integer\n");
            return 1;
        }
        
        *(data + i) = value;
    }
    
    double results[4];
    compute_statistics(data, (size_t)count, results);
    
    printf("\nStatistical Results:\n");
    printf("Mean: %.2f\n", results[0]);
    printf("Standard Deviation: %.2f\n", results[1]);
    printf("Minimum: %.0f\n", results[2]);
    printf("Maximum: %.0f\n", results[3]);
    
    return 0;
}