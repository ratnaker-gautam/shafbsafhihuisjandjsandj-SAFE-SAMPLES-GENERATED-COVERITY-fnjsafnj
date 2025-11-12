//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void process_array(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int* start = arr;
    int* end = arr + size - 1;
    
    while (start < end) {
        int temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

int calculate_sum(const int* arr, size_t size) {
    if (arr == NULL || size == 0) return 0;
    
    const int* ptr = arr;
    const int* end = arr + size;
    int sum = 0;
    
    while (ptr < end) {
        if ((*ptr > 0 && sum > INT_MAX - *ptr) || 
            (*ptr < 0 && sum < INT_MIN - *ptr)) {
            return 0;
        }
        sum += *ptr;
        ptr++;
    }
    
    return sum;
}

int main() {
    int numbers[ARRAY_SIZE];
    char input_buffer[32];
    
    printf("Enter %d integers:\n", ARRAY_SIZE);
    
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        printf("Number %zu: ", i + 1);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        } else if (len == sizeof(input_buffer) - 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            fprintf(stderr, "Input too long\n");
            return 1;
        }
        
        int value;
        if (validate_input(input_buffer, &value) != 0) {
            fprintf(stderr, "Invalid input: must be an integer\n");
            return 1;
        }
        
        numbers[i] = value;
    }
    
    printf("\nOriginal array:\n");
    int* ptr = numbers;
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");
    
    process_array(numbers, ARRAY_SIZE);
    
    printf("\nReversed array:\n");
    ptr = numbers;
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");
    
    int sum = calculate_sum(numbers, ARRAY_SIZE);
    printf("\nSum of reversed array: %d\n", sum);
    
    int* max_ptr = numbers;
    int* current = numbers + 1;
    while (current < numbers + ARRAY_SIZE) {
        if (*current > *max_ptr) {
            max_ptr = current;
        }
        current++;
    }
    
    printf("Maximum value in reversed array: %d\n", *max_ptr);
    
    return 0;
}