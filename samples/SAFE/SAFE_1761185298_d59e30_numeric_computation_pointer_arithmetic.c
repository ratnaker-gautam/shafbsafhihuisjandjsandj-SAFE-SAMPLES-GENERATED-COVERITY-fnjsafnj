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

void compute_statistics(int* arr, size_t size, int* min, int* max, double* avg) {
    if (arr == NULL || min == NULL || max == NULL || avg == NULL || size == 0) return;
    
    *min = *arr;
    *max = *arr;
    long long sum = 0;
    
    int* ptr = arr;
    int* end = arr + size;
    
    while (ptr < end) {
        if (*ptr < *min) *min = *ptr;
        if (*ptr > *max) *max = *ptr;
        sum += *ptr;
        ptr++;
    }
    
    *avg = (double)sum / size;
}

void fill_array_with_primes(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int count = 0;
    int num = 2;
    int* ptr = arr;
    
    while (count < size) {
        int is_prime = 1;
        
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        
        if (is_prime) {
            *ptr = num;
            ptr++;
            count++;
        }
        num++;
    }
}

int main() {
    int array[ARRAY_SIZE];
    fill_array_with_primes(array, ARRAY_SIZE);
    
    int min_val, max_val;
    double average;
    compute_statistics(array, ARRAY_SIZE, &min_val, &max_val, &average);
    
    printf("Array statistics for first %d prime numbers:\n", ARRAY_SIZE);
    printf("Minimum: %d\n", min_val);
    printf("Maximum: %d\n", max_val);
    printf("Average: %.2f\n", average);
    
    printf("\nEnter an index to retrieve (0-%d): ", ARRAY_SIZE - 1);
    char input[50];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    int index;
    if (validate_input(input, &index) != 0) {
        printf("Invalid input: must be an integer\n");
        return 1;
    }
    
    if (index < 0 || index >= ARRAY_SIZE) {
        printf("Index out of bounds: must be between 0 and %d\n", ARRAY_SIZE - 1);
        return 1;
    }
    
    int* element_ptr = array + index;
    printf("Element at index %d: %d\n", index, *element_ptr);
    
    printf("\nFirst 10 elements using pointer arithmetic:\n");
    int* ptr = array;
    for (int i = 0; i < 10 && i < ARRAY_SIZE; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}