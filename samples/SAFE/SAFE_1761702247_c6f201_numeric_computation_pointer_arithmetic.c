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
        if (value > 0 && value > INT_MAX - 10) value = 1;
        if (value < 0 && value < INT_MIN + 10) value = -1;
        
        *current = value;
        value += 2;
        current++;
    }
}

int compute_sum(const int* arr, size_t size) {
    if (arr == NULL || size == 0) return 0;
    
    const int* end = arr + size;
    const int* current = arr;
    long long sum = 0;
    
    while (current < end) {
        if (*current > 0 && sum > LLONG_MAX - *current) {
            sum = LLONG_MAX;
            break;
        }
        if (*current < 0 && sum < LLONG_MIN - *current) {
            sum = LLONG_MIN;
            break;
        }
        sum += *current;
        current++;
    }
    
    if (sum > INT_MAX) return INT_MAX;
    if (sum < INT_MIN) return INT_MIN;
    return (int)sum;
}

int find_max(const int* arr, size_t size) {
    if (arr == NULL || size == 0) return 0;
    
    const int* end = arr + size;
    const int* current = arr;
    int max_val = *arr;
    
    while (current < end) {
        if (*current > max_val) {
            max_val = *current;
        }
        current++;
    }
    
    return max_val;
}

int main() {
    char input_buffer[256];
    long start_value;
    
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
        fprintf(stderr, "Invalid input. Please enter a valid integer.\n");
        return 1;
    }
    
    int array[ARRAY_SIZE];
    fill_array(array, ARRAY_SIZE, (int)start_value);
    
    int sum = compute_sum(array, ARRAY_SIZE);
    int max_val = find_max(array, ARRAY_SIZE);
    
    printf("Array filled with %d elements starting from %d\n", ARRAY_SIZE, (int)start_value);
    printf("Sum of all elements: %d\n", sum);
    printf("Maximum value in array: %d\n", max_val);
    
    printf("First 10 elements: ");
    const int* ptr = array;
    for (int i = 0; i < 10 && i < ARRAY_SIZE; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");
    
    return 0;
}