//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 100

int validate_integer_input(const char* input, int* value) {
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        return 0;
    }
    
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    
    *value = (int)val;
    return 1;
}

void fill_array_with_primes(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int count = 0;
    int num = 2;
    
    while (count < size) {
        int is_prime = 1;
        
        for (int i = 2; i <= (int)sqrt(num); i++) {
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
    
    double sum = 0.0;
    double sum_sq = 0.0;
    
    for (size_t i = 0; i < size; i++) {
        sum += *(arr + i);
        sum_sq += (double)(*(arr + i)) * (double)(*(arr + i));
    }
    
    *mean = sum / size;
    *stddev = sqrt((sum_sq / size) - (*mean * *mean));
    
    return *stddev;
}

int main() {
    int array[ARRAY_SIZE];
    int* ptr = array;
    
    fill_array_with_primes(ptr, ARRAY_SIZE);
    
    printf("First 10 prime numbers in array:\n");
    for (int i = 0; i < 10 && i < ARRAY_SIZE; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");
    
    double mean, stddev;
    compute_statistics(ptr, ARRAY_SIZE, &mean, &stddev);
    
    printf("Statistics for first %d prime numbers:\n", ARRAY_SIZE);
    printf("Mean: %.2f\n", mean);
    printf("Standard deviation: %.2f\n", stddev);
    
    printf("\nEnter an index to retrieve prime (0-%d): ", ARRAY_SIZE - 1);
    char input[50];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int index;
    if (!validate_integer_input(input, &index)) {
        printf("Invalid input: must be an integer\n");
        return 1;
    }
    
    if (index < 0 || index >= ARRAY_SIZE) {
        printf("Index out of bounds\n");
        return 1;
    }
    
    printf("Prime at index %d: %d\n", index, *(ptr + index));
    
    printf("\nPointer arithmetic demonstration:\n");
    printf("Base address: %p\n", (void*)ptr);
    printf("Address of element 50: %p\n", (void*)(ptr + 50));
    printf("Difference in bytes: %ld\n", ((char*)(ptr + 50) - (char*)ptr));
    
    return 0;
}