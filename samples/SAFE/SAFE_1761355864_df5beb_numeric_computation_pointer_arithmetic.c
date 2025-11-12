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
    if (arr == NULL || size == 0 || min == NULL || max == NULL || avg == NULL) return;
    
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
    
    *avg = (double)sum / (double)size;
}

void fill_array_with_primes(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int count = 0;
    int num = 2;
    int* ptr = arr;
    
    while (count < (int)size) {
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
    
    printf("Generated first %d prime numbers:\n", ARRAY_SIZE);
    
    int* ptr = array;
    int* end = array + ARRAY_SIZE;
    int count = 0;
    
    while (ptr < end) {
        printf("%6d", *ptr);
        count++;
        if (count % 10 == 0) printf("\n");
        ptr++;
    }
    
    int min_val, max_val;
    double avg_val;
    compute_statistics(array, ARRAY_SIZE, &min_val, &max_val, &avg_val);
    
    printf("\nArray Statistics:\n");
    printf("Minimum: %d\n", min_val);
    printf("Maximum: %d\n", max_val);
    printf("Average: %.2f\n", avg_val);
    
    printf("\nEnter a number to find in the array: ");
    char input[256];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    int search_num;
    if (validate_input(input, &search_num) != 0) {
        printf("Invalid input: must be an integer\n");
        return 1;
    }
    
    int found = 0;
    int position = -1;
    ptr = array;
    
    while (ptr < end) {
        if (*ptr == search_num) {
            found = 1;
            position = (int)(ptr - array);
            break;
        }
        ptr++;
    }
    
    if (found) {
        printf("Number %d found at position %d\n", search_num, position);
    } else {
        printf("Number %d not found in the array\n", search_num);
    }
    
    return 0;
}