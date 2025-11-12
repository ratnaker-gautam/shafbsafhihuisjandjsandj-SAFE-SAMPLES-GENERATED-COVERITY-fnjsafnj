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

void compute_statistics(int* arr, size_t size, double* results) {
    if (arr == NULL || results == NULL || size == 0) return;
    
    int* ptr = arr;
    int* end = arr + size;
    
    int sum = 0;
    int min = INT_MAX;
    int max = INT_MIN;
    
    while (ptr < end) {
        sum += *ptr;
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        ptr++;
    }
    
    results[0] = (double)sum / size;
    results[1] = (double)min;
    results[2] = (double)max;
    
    double variance = 0.0;
    ptr = arr;
    while (ptr < end) {
        double diff = *ptr - results[0];
        variance += diff * diff;
        ptr++;
    }
    results[3] = variance / size;
}

int main() {
    char buffer[256];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    if (validate_input(buffer, &size) != 0) {
        fprintf(stderr, "Invalid size\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    printf("Enter %d integers:\n", size);
    
    for (int i = 0; i < size; i++) {
        printf("Element %d: ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        int value;
        if (validate_input(buffer, &value) != 0) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        
        arr[i] = value;
    }
    
    double results[4];
    compute_statistics(arr, size, results);
    
    printf("\nResults:\n");
    printf("Mean: %.2f\n", results[0]);
    printf("Min: %.2f\n", results[1]);
    printf("Max: %.2f\n", results[2]);
    printf("Variance: %.2f\n", results[3]);
    
    return 0;
}