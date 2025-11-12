//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_SIZE 100

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
        if (*current % 2 == 0) {
            *current *= 2;
        } else {
            *current += 1;
        }
        current++;
    }
}

void print_array(const int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    const int* end = arr + size;
    const int* current = arr;
    
    while (current < end) {
        printf("%d", *current);
        if (current + 1 < end) {
            printf(" ");
        }
        current++;
    }
    printf("\n");
}

int main() {
    char input[256];
    int numbers[MAX_SIZE];
    size_t count = 0;
    
    printf("Enter up to %d integers (one per line, empty line to finish):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (input[0] == '\0') {
            break;
        }
        
        int value;
        if (validate_input(input, &value) == 0) {
            numbers[count] = value;
            count++;
        } else {
            printf("Invalid input: %s\n", input);
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 0;
    }
    
    printf("Original array: ");
    print_array(numbers, count);
    
    process_array(numbers, count);
    
    printf("Processed array: ");
    print_array(numbers, count);
    
    int sum = 0;
    int* end = numbers + count;
    int* current = numbers;
    
    while (current < end) {
        if (*current > 0 && sum > INT_MAX - *current) {
            printf("Sum would overflow, stopping calculation.\n");
            break;
        }
        if (*current < 0 && sum < INT_MIN - *current) {
            printf("Sum would underflow, stopping calculation.\n");
            break;
        }
        sum += *current;
        current++;
    }
    
    printf("Sum of processed array: %d\n", sum);
    
    return 0;
}