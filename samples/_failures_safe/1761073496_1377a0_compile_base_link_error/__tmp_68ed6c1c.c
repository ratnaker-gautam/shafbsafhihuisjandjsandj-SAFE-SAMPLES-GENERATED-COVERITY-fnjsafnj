//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 100

int validate_input(const char* input, int* result) {
    if (input == NULL || result == NULL) return 0;
    
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input) return 0;
    if (*endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    
    *result = (int)val;
    return 1;
}

void fill_array(int* arr, size_t size, int start_val) {
    if (arr == NULL) return;
    
    int* end = arr + size;
    int* current = arr;
    int value = start_val;
    
    while (current < end) {
        if (value > 0 && value > INT_MAX / 2) value = 1;
        *current = value * value;
        value++;
        current++;
    }
}

double compute_mean(const int* arr, size_t size) {
    if (arr == NULL || size == 0) return 0.0;
    
    const int* end = arr + size;
    const int* current = arr;
    double sum = 0.0;
    size_t count = 0;
    
    while (current < end) {
        sum += *current;
        count++;
        current++;
    }
    
    if (count == 0) return 0.0;
    return sum / count;
}

double compute_stddev(const int* arr, size_t size, double mean) {
    if (arr == NULL || size == 0) return 0.0;
    
    const int* end = arr + size;
    const int* current = arr;
    double sum_sq_diff = 0.0;
    size_t count = 0;
    
    while (current < end) {
        double diff = *current - mean;
        sum_sq_diff += diff * diff;
        count++;
        current++;
    }
    
    if (count <= 1) return 0.0;
    return sqrt(sum_sq_diff / (count - 1));
}

int main(void) {
    char input_buffer[64];
    int start_value;
    
    printf("Enter starting integer value: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer, &start_value)) {
        fprintf(stderr, "Invalid input: must be a valid integer\n");
        return 1;
    }
    
    int data_array[ARRAY_SIZE];
    fill_array(data_array, ARRAY_SIZE, start_value);
    
    double mean_val = compute_mean(data_array, ARRAY_SIZE);
    double stddev_val = compute_stddev(data_array, ARRAY_SIZE, mean_val);
    
    printf("Array statistics:\n");
    printf("Size: %d\n", ARRAY_SIZE);
    printf("Mean: %.2f\n", mean_val);
    printf("Standard deviation: %.2f\n", stddev_val);
    
    printf("\nFirst 10 elements:\n");
    const int* end = data_array + 10;
    const int* current = data_array;
    while (current < end && current < data_array + ARRAY_SIZE) {
        printf("%d ", *current);
        current++;
    }
    printf("\n");
    
    return 0;
}