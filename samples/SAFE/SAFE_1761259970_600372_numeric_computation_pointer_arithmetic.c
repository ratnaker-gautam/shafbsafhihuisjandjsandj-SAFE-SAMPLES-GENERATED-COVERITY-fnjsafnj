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
    
    if (endptr == input) return 0;
    if (*endptr != '\0') return 0;
    if (value < INT_MIN || value > INT_MAX) return 0;
    
    *result = value;
    return 1;
}

void process_array(int* arr, size_t size, int factor) {
    if (arr == NULL || size == 0) return;
    
    int* end = arr + size;
    int* current = arr;
    
    while (current < end) {
        if (*current > 0) {
            if (factor > 0 && *current > INT_MAX / factor) {
                *current = INT_MAX;
            } else if (factor < 0 && *current < INT_MIN / factor) {
                *current = INT_MIN;
            } else {
                *current *= factor;
            }
        }
        current++;
    }
}

void print_array(const int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    const int* end = arr + size;
    const int* current = arr;
    
    while (current < end) {
        printf("%d ", *current);
        current++;
    }
    printf("\n");
}

int main() {
    int numbers[ARRAY_SIZE];
    char input_buffer[256];
    long factor_value;
    
    printf("Enter a multiplication factor (integer): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer, &factor_value)) {
        fprintf(stderr, "Invalid input. Please enter a valid integer.\n");
        return 1;
    }
    
    int factor = (int)factor_value;
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        numbers[i] = i + 1;
    }
    
    printf("Original array (first 20 elements): ");
    print_array(numbers, 20);
    
    process_array(numbers, ARRAY_SIZE, factor);
    
    printf("Processed array (first 20 elements): ");
    print_array(numbers, 20);
    
    int sum = 0;
    int* end = numbers + ARRAY_SIZE;
    for (int* ptr = numbers; ptr < end; ptr++) {
        if (*ptr > 0 && sum <= INT_MAX - *ptr) {
            sum += *ptr;
        } else if (*ptr < 0 && sum >= INT_MIN - *ptr) {
            sum += *ptr;
        } else {
            if (*ptr > 0) sum = INT_MAX;
            else sum = INT_MIN;
            break;
        }
    }
    
    printf("Sum of all elements: %d\n", sum);
    
    return 0;
}