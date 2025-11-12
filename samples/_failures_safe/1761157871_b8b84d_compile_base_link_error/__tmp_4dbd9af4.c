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
    
    *valid = (int)value;
    return 0;
}

void compute_statistics(int* arr, size_t size, double* results) {
    if (arr == NULL || results == NULL || size == 0) return;
    
    int* ptr = arr;
    int* end = arr + size;
    
    long long sum = 0;
    int min = INT_MAX;
    int max = INT_MIN;
    
    while (ptr < end) {
        sum += *ptr;
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        ptr++;
    }
    
    double mean = (double)sum / size;
    
    ptr = arr;
    double variance_sum = 0.0;
    while (ptr < end) {
        double diff = *ptr - mean;
        variance_sum += diff * diff;
        ptr++;
    }
    
    double variance = variance_sum / size;
    double std_dev = sqrt(variance);
    
    results[0] = mean;
    results[1] = variance;
    results[2] = std_dev;
    results[3] = (double)min;
    results[4] = (double)max;
}

void fill_array(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int* ptr = arr;
    int* end = arr + size;
    
    while (ptr < end) {
        *ptr = (int)(ptr - arr) * 2 + 1;
        ptr++;
    }
}

void print_array(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int* ptr = arr;
    int* end = arr + size;
    
    printf("Array contents: [");
    while (ptr < end) {
        printf("%d", *ptr);
        if (ptr < end - 1) printf(", ");
        ptr++;
    }
    printf("]\n");
}

int main() {
    char input_buffer[256];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (validate_input(input_buffer, &size) != 0) {
        fprintf(stderr, "Invalid input. Please enter a number between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int array[MAX_SIZE];
    double results[5];
    
    fill_array(array, size);
    print_array(array, size);
    compute_statistics(array, size, results);
    
    printf("Statistics:\n");
    printf("Mean: %.2f\n", results[0]);
    printf("Variance: %.2f\n", results[1]);
    printf("Standard Deviation: %.2f\n", results[2]);
    printf("Minimum: %.0f\n", results[3]);
    printf("Maximum: %.0f\n", results[4]);
    
    return 0;
}