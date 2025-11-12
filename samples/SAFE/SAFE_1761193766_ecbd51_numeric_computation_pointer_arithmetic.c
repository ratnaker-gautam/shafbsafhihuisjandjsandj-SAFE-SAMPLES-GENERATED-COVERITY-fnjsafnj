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

void compute_statistics(int* arr, size_t size, double* mean, double* stddev) {
    if (arr == NULL || mean == NULL || stddev == NULL || size == 0) return;
    
    int64_t sum = 0;
    int64_t sum_sq = 0;
    
    for (size_t i = 0; i < size; i++) {
        sum += *(arr + i);
        sum_sq += (int64_t)*(arr + i) * (int64_t)*(arr + i);
    }
    
    *mean = (double)sum / (double)size;
    double variance = ((double)sum_sq / (double)size) - (*mean * *mean);
    *stddev = sqrt(variance > 0.0 ? variance : 0.0);
}

int main() {
    int numbers[ARRAY_SIZE];
    char input_buffer[64];
    
    printf("Enter %d integers:\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("Number %d: ", i + 1);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        int64_t value;
        if (!validate_input(input_buffer, &value)) {
            fprintf(stderr, "Invalid input. Please enter a valid integer.\n");
            i--;
            continue;
        }
        
        *(numbers + i) = (int)value;
    }
    
    double mean, stddev;
    compute_statistics(numbers, ARRAY_SIZE, &mean, &stddev);
    
    printf("\nComputed Statistics:\n");
    printf("Mean: %.2f\n", mean);
    printf("Standard Deviation: %.2f\n", stddev);
    
    printf("\nNumbers greater than mean:\n");
    int count_above = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (*(numbers + i) > mean) {
            printf("%d ", *(numbers + i));
            count_above++;
        }
    }
    printf("\nTotal above mean: %d\n", count_above);
    
    return 0;
}