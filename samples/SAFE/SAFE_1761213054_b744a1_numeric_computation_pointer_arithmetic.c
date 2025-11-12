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

void process_array(int* arr, size_t size, int multiplier) {
    if (arr == NULL || size == 0) return;
    
    int* end = arr + size;
    int* current = arr;
    
    while (current < end) {
        if (multiplier > 0 && *current > INT_MAX / multiplier) {
            *current = INT_MAX;
        } else if (multiplier < 0 && *current < INT_MIN / multiplier) {
            *current = INT_MIN;
        } else {
            *current = *current * multiplier;
        }
        current++;
    }
}

void fill_array(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int* end = arr + size;
    int* current = arr;
    int value = 1;
    
    while (current < end) {
        *current = value;
        value = (value * 3 + 1) % 100;
        current++;
    }
}

void print_array(const int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    const int* end = arr + size;
    const int* current = arr;
    
    printf("Array contents:\n");
    while (current < end) {
        printf("%d ", *current);
        current++;
        if ((current - arr) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <multiplier>\n", argv[0]);
        return 1;
    }
    
    long multiplier_long;
    if (!validate_input(argv[1], &multiplier_long)) {
        printf("Error: Invalid multiplier. Must be a valid integer.\n");
        return 1;
    }
    
    int multiplier = (int)multiplier_long;
    
    int array[ARRAY_SIZE];
    fill_array(array, ARRAY_SIZE);
    
    printf("Original array:\n");
    print_array(array, ARRAY_SIZE);
    
    process_array(array, ARRAY_SIZE, multiplier);
    
    printf("After multiplying by %d:\n", multiplier);
    print_array(array, ARRAY_SIZE);
    
    int sum = 0;
    int* end = array + ARRAY_SIZE;
    int* current = array;
    
    while (current < end) {
        if (*current > 0 && sum > INT_MAX - *current) {
            sum = INT_MAX;
            break;
        } else if (*current < 0 && sum < INT_MIN - *current) {
            sum = INT_MIN;
            break;
        } else {
            sum += *current;
        }
        current++;
    }
    
    printf("Sum of all elements: %d\n", sum);
    
    return 0;
}