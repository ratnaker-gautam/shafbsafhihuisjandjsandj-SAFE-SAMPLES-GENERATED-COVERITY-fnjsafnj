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

void compute_statistics(int* arr, size_t size, int* sum, double* avg, int* min, int* max) {
    if (arr == NULL || size == 0 || sum == NULL || avg == NULL || min == NULL || max == NULL) return;
    
    *sum = 0;
    *min = INT_MAX;
    *max = INT_MIN;
    
    int* ptr = arr;
    int* end = arr + size;
    
    while (ptr < end) {
        if (*ptr > *max) *max = *ptr;
        if (*ptr < *min) *min = *ptr;
        
        if (*sum > 0 && *ptr > INT_MAX - *sum) {
            *sum = INT_MAX;
        } else if (*sum < 0 && *ptr < INT_MIN - *sum) {
            *sum = INT_MIN;
        } else {
            *sum += *ptr;
        }
        
        ptr++;
    }
    
    *avg = (size > 0) ? (double)*sum / (double)size : 0.0;
}

int main() {
    int numbers[ARRAY_SIZE];
    size_t count = 0;
    char input[256];
    
    printf("Enter up to %d integers (one per line, empty line to finish):\n", ARRAY_SIZE);
    
    while (count < ARRAY_SIZE) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        int value;
        if (validate_input(input, &value) == 0) {
            numbers[count] = value;
            count++;
        } else {
            printf("Invalid input. Please enter a valid integer.\n");
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 0;
    }
    
    int sum, min, max;
    double average;
    
    compute_statistics(numbers, count, &sum, &average, &min, &max);
    
    printf("\nResults:\n");
    printf("Count: %zu\n", count);
    printf("Sum: %d\n", sum);
    printf("Average: %.2f\n", average);
    printf("Minimum: %d\n", min);
    printf("Maximum: %d\n", max);
    
    printf("\nArray contents (using pointer arithmetic):\n");
    int* ptr = numbers;
    int* end = numbers + count;
    while (ptr < end) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}