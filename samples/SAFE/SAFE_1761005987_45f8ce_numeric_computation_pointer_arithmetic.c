//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
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
    char buffer[256];
    int numbers[MAX_SIZE];
    size_t count = 0;
    
    printf("Enter up to %d integers (empty line to finish):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') break;
        
        int value;
        if (validate_input(buffer, &value) == 0) {
            numbers[count] = value;
            count++;
        } else {
            printf("Invalid input: %s\n", buffer);
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    printf("Original array: ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    process_array(numbers, count);
    
    printf("Reversed array: ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    int sum = calculate_sum(numbers, count);
    printf("Sum of reversed array: %d\n", sum);
    
    return 0;
}