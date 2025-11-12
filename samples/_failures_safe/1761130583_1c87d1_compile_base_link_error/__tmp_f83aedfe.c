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
    
    double sum = 0.0;
    double sum_sq = 0.0;
    
    int* ptr = arr;
    int* end = arr + size;
    
    while (ptr < end) {
        sum += *ptr;
        sum_sq += (double)(*ptr) * (double)(*ptr);
        ptr++;
    }
    
    *mean = sum / size;
    *stddev = sqrt((sum_sq / size) - (*mean * *mean));
}

void fill_array_with_pattern(int* arr, size_t size, int start_val) {
    if (arr == NULL || size == 0) return;
    
    int* ptr = arr;
    int* end = arr + size;
    int val = start_val;
    
    while (ptr < end) {
        *ptr = val;
        val = (val * 3 + 7) % 100;
        ptr++;
    }
}

int main() {
    char input_buffer[64];
    int64_t start_value;
    int array[ARRAY_SIZE];
    
    printf("Enter starting value for array pattern (-10000 to 10000): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer, &start_value)) {
        fprintf(stderr, "Invalid input. Please enter a valid integer.\n");
        return 1;
    }
    
    if (start_value < -10000 || start_value > 10000) {
        fprintf(stderr, "Value out of range. Please enter between -10000 and 10000.\n");
        return 1;
    }
    
    fill_array_with_pattern(array, ARRAY_SIZE, (int)start_value);
    
    double mean, stddev;
    compute_statistics(array, ARRAY_SIZE, &mean, &stddev);
    
    printf("Array statistics:\n");
    printf("Mean: %.2f\n", mean);
    printf("Standard deviation: %.2f\n", stddev);
    
    printf("First 10 array elements: ");
    int* ptr = array;
    for (int i = 0; i < 10 && ptr < array + ARRAY_SIZE; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}