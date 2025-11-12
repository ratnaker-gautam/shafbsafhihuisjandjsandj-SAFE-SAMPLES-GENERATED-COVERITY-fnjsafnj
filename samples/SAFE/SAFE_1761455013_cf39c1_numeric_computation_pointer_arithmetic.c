//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    
    int* end = arr + size;
    int* current = arr;
    
    while (current < end) {
        int* next = current + 1;
        if (next < end) {
            if ((*current > 0 && *next > INT_MAX - *current) ||
                (*current < 0 && *next < INT_MIN - *current)) {
                *current = 0;
            } else {
                *current = *current + *next;
            }
        }
        current++;
    }
}

void reverse_array(int* arr, size_t size) {
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

int main() {
    int array[ARRAY_SIZE];
    
    printf("Enter %d integers:\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        int value;
        if (validate_input(buffer, &value) != 0) {
            fprintf(stderr, "Invalid input: %s\n", buffer);
            return 1;
        }
        
        array[i] = value;
    }
    
    printf("Original array:\n");
    int* ptr = array;
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");
    
    process_array(array, ARRAY_SIZE);
    
    printf("After processing:\n");
    ptr = array;
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");
    
    reverse_array(array, ARRAY_SIZE);
    
    printf("After reversal:\n");
    ptr = array;
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");
    
    int sum = 0;
    ptr = array;
    int* end = array + ARRAY_SIZE;
    while (ptr < end) {
        if ((*ptr > 0 && sum > INT_MAX - *ptr) ||
            (*ptr < 0 && sum < INT_MIN - *ptr)) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum += *ptr;
        ptr++;
    }
    
    printf("Sum of all elements: %d\n", sum);
    
    return 0;
}