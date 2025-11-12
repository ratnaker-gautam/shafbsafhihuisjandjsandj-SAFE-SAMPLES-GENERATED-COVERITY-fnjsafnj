//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int validate_input(const char* input, int* valid) {
    if (input == NULL || valid == NULL) return -1;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return -1;
    if (value < 1 || value > MAX_SIZE) return -1;
    
    *valid = (int)value;
    return 0;
}

void compute_statistics(int* data, size_t size, double* results) {
    if (data == NULL || results == NULL || size == 0) return;
    
    int* ptr = data;
    int* end = data + size;
    
    double sum = 0.0;
    int min = *ptr;
    int max = *ptr;
    
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
            *current = num;
            current++;
            count++;
        }
        num++;
    }
}

int main() {
    char input[32];
    int size;
    
    printf("Enter the number of prime numbers to generate (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input. Please enter a number between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int primes[MAX_SIZE];
    fill_with_primes(primes, size);
    
    printf("First %d prime numbers:\n", size);
    int* ptr = primes;
    for (int i = 0; i < size; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    double stats[4];
    compute_statistics(primes, size, stats);
    
    printf("Statistics:\n");
    printf("Mean: %.2f\n", stats[0]);
    printf("Standard Deviation: %.2f\n", stats[1]);
    printf("Minimum: %.0f\n", stats[2]);
    printf("Maximum: %.0f\n", stats[3]);
    
    return 0;
}