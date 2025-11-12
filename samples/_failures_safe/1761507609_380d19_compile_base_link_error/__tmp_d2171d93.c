//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 100

int validate_integer_input(const char* input, int* result) {
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        return 0;
    }
    
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    
    *result = (int)val;
    return 1;
}

void fill_array_with_primes(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int count = 0;
    int num = 2;
    
    while (count < size) {
        int is_prime = 1;
        
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        
        if (is_prime) {
            *(arr + count) = num;
            count++;
        }
        num++;
    }
}

double compute_statistics(int* arr, size_t size, double* mean, double* stddev) {
    if (arr == NULL || size == 0 || mean == NULL || stddev == NULL) {
        return 0.0;
    }
    
    long long sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += *(arr + i);
    }
    *mean = (double)sum / size;
    
    double variance_sum = 0.0;
    for (size_t i = 0; i < size; i++) {
        double diff = *(arr + i) - *mean;
        variance_sum += diff * diff;
    }
    *stddev = sqrt(variance_sum / size);
    
    double median;
    if (size % 2 == 0) {
        median = (*(arr + size/2 - 1) + *(arr + size/2)) / 2.0;
    } else {
        median = *(arr + size/2);
    }
    
    return median;
}

int main() {
    int array[ARRAY_SIZE];
    int* arr_ptr = array;
    
    fill_array_with_primes(arr_ptr, ARRAY_SIZE);
    
    printf("Generated first %d prime numbers:\n", ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", *(arr_ptr + i));
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    
    double mean, stddev;
    double median = compute_statistics(arr_ptr, ARRAY_SIZE, &mean, &stddev);
    
    printf("Statistical analysis:\n");
    printf("Mean: %.2f\n", mean);
    printf("Median: %.2f\n", median);
    printf("Standard deviation: %.2f\n", stddev);
    
    printf("\nEnter an index to retrieve a prime (0-%d): ", ARRAY_SIZE - 1);
    char input_buffer[256];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int index;
    if (!validate_integer_input(input_buffer, &index)) {
        printf("Invalid input: must be an integer\n");
        return 1;
    }
    
    if (index < 0 || index >= ARRAY_SIZE) {
        printf("Index out of bounds: must be between 0 and %d\n", ARRAY_SIZE - 1);
        return 1;
    }
    
    printf("Prime at index %d: %d\n", index, *(arr_ptr + index));
    
    printf("\nPointer arithmetic demonstration:\n");
    printf("Base address: %p\n", (void*)arr_ptr);
    printf("Address of element at index %d: %p\n", index, (void*)(arr_ptr + index));
    printf("Offset from base: %td elements\n", (arr_ptr + index) - arr_ptr);
    
    return 0;
}