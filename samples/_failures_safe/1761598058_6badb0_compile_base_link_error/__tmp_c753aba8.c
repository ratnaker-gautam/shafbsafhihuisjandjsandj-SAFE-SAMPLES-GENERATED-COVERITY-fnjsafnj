//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 100

int validate_input(const char* input, int* valid) {
    if (input == NULL || valid == NULL) return -1;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return -1;
    if (value < 1 || value > MAX_SIZE) return -1;
    if (value > INT_MAX) return -1;
    
    *valid = (int)value;
    return 0;
}

void compute_statistics(int* data, size_t size, double* results) {
    if (data == NULL || results == NULL || size == 0) return;
    
    int* ptr = data;
    int* end = data + size;
    
    double sum = 0.0;
    int min = INT_MAX;
    int max = INT_MIN;
    
    while (ptr < end) {
        sum += *ptr;
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        ptr++;
    }
    
    double mean = sum / size;
    
    ptr = data;
    double variance = 0.0;
    while (ptr < end) {
        double diff = *ptr - mean;
        variance += diff * diff;
        ptr++;
    }
    variance /= size;
    
    results[0] = mean;
    results[1] = sqrt(variance);
    results[2] = (double)min;
    results[3] = (double)max;
}

void fill_with_primes(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int* current = arr;
    int* end = arr + size;
    int num = 2;
    int count = 0;
    
    while (current < end && count < size) {
        int is_prime = 1;
        
        for (int i = 2; i <= (int)sqrt(num); i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        
        if (is_prime) {
            *current = num;
            current++;
            count++;
        }
        num++;
        
        if (num < 0) break;
    }
}

int main() {
    char input[32];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input. Please enter a number between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int data[MAX_SIZE];
    fill_with_primes(data, size);
    
    printf("Generated prime numbers:\n");
    int* ptr = data;
    int* end = data + size;
    while (ptr < end) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    double stats[4];
    compute_statistics(data, size, stats);
    
    printf("Statistics:\n");
    printf("Mean: %.2f\n", stats[0]);
    printf("Std Dev: %.2f\n", stats[1]);
    printf("Min: %.0f\n", stats[2]);
    printf("Max: %.0f\n", stats[3]);
    
    return 0;
}