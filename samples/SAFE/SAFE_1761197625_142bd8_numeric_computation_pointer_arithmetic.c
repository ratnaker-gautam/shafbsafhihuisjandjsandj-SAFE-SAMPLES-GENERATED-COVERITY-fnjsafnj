//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ARRAY_SIZE 100

int validate_input(const char* input, long* result) {
    if (input == NULL || result == NULL) return 0;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (value < INT_MIN || value > INT_MAX) return 0;
    
    *result = value;
    return 1;
}

void fill_array(int* arr, size_t size, int start_value) {
    if (arr == NULL || size == 0) return;
    
    int* end = arr + size;
    int* current = arr;
    int value = start_value;
    
    while (current < end) {
        if (value > 0 && value > INT_MAX / 2) value = 1;
        *current = value;
        value *= 2;
        current++;
    }
}

int compute_sum(const int* arr, size_t size) {
    if (arr == NULL || size == 0) return 0;
    
    const int* end = arr + size;
    const int* current = arr;
    int sum = 0;
    
    while (current < end) {
        if ((*current > 0 && sum > INT_MAX - *current) ||
            (*current < 0 && sum < INT_MIN - *current)) {
            return 0;
        }
        sum += *current;
        current++;
    }
    
    return sum;
}

double compute_average(const int* arr, size_t size) {
    if (arr == NULL || size == 0) return 0.0;
    
    int sum = compute_sum(arr, size);
    if (sum == 0 && size > 0) return 0.0;
    
    return (double)sum / (double)size;
}

int find_max(const int* arr, size_t size) {
    if (arr == NULL || size == 0) return 0;
    
    const int* end = arr + size;
    const int* current = arr;
    int max_value = *current;
    current++;
    
    while (current < end) {
        if (*current > max_value) {
            max_value = *current;
        }
        current++;
    }
    
    return max_value;
}

int main(void) {
    int array[ARRAY_SIZE];
    char input_buffer[64];
    long start_value;
    
    printf("Enter starting value for array generation: ");
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
    
    fill_array(array, ARRAY_SIZE, (int)start_value);
    
    printf("Array statistics:\n");
    printf("Size: %d\n", ARRAY_SIZE);
    printf("Sum: %d\n", compute_sum(array, ARRAY_SIZE));
    printf("Average: %.2f\n", compute_average(array, ARRAY_SIZE));
    printf("Maximum value: %d\n", find_max(array, ARRAY_SIZE));
    
    printf("\nFirst 10 elements: ");
    int* ptr = array;
    for (int i = 0; i < 10 && i < ARRAY_SIZE; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}