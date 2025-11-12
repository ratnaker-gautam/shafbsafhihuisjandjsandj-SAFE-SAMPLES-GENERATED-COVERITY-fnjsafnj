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
    if (arr == NULL || size == 0 || size > MAX_SIZE) return;
    
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

int compute_sum(const int* arr, size_t size) {
    if (arr == NULL || size == 0 || size > MAX_SIZE) return 0;
    
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
    int array[MAX_SIZE];
    size_t count = 0;
    char buffer[256];
    
    printf("Enter up to %d integers (empty line to finish):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        if (buffer[0] == '\n') break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) break;
        
        int value;
        if (validate_input(buffer, &value) == 0) {
            array[count] = value;
            count++;
        } else {
            printf("Invalid input. Please enter an integer.\n");
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    printf("Original array: ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    process_array(array, count);
    
    printf("Reversed array: ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    int total = compute_sum(array, count);
    printf("Sum of reversed array: %d\n", total);
    
    return 0;
}