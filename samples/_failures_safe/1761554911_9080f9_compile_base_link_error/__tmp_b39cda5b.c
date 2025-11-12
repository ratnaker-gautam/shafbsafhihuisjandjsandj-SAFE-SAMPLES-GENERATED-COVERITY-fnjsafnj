//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 100

int validate_input(const char* input, int64_t* result) {
    if (input == NULL || result == NULL) return 0;
    
    char* endptr;
    long long val = strtoll(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    
    *result = (int64_t)val;
    return 1;
}

void compute_statistics(int* arr, size_t size, double* results) {
    if (arr == NULL || results == NULL || size == 0) return;
    
    int* ptr = arr;
    int* end = arr + size;
    
    int64_t sum = 0;
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
    double stddev = sqrt(variance);
    
    results[0] = mean;
    results[1] = variance;
    results[2] = stddev;
    results[3] = (double)min;
    results[4] = (double)max;
}

int main() {
    int numbers[ARRAY_SIZE];
    double stats[5];
    
    printf("Enter %d integers:\n", ARRAY_SIZE);
    
    char buffer[256];
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        printf("Number %zu: ", i + 1);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        int64_t value;
        if (!validate_input(buffer, &value)) {
            fprintf(stderr, "Invalid input. Please enter a valid integer.\n");
            i--;
            continue;
        }
        
        numbers[i] = (int)value;
    }
    
    compute_statistics(numbers, ARRAY_SIZE, stats);
    
    printf("\nStatistical Analysis:\n");
    printf("Mean: %.2f\n", stats[0]);
    printf("Variance: %.2f\n", stats[1]);
    printf("Standard Deviation: %.2f\n", stats[2]);
    printf("Minimum: %.0f\n", stats[3]);
    printf("Maximum: %.0f\n", stats[4]);
    
    return 0;
}