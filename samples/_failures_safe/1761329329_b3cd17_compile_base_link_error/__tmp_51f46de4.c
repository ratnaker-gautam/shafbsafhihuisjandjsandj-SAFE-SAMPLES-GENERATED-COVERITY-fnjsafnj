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
    
    *valid = (int)value;
    return 0;
}

void compute_statistics(int* data, size_t count, double* results) {
    if (data == NULL || results == NULL || count == 0) return;
    
    int* ptr = data;
    int* end = data + count;
    
    long long sum = 0;
    long long sum_sq = 0;
    int min = INT_MAX;
    int max = INT_MIN;
    
    while (ptr < end) {
        int val = *ptr;
        sum += val;
        sum_sq += (long long)val * val;
        if (val < min) min = val;
        if (val > max) max = val;
        ptr++;
    }
    
    double mean = (double)sum / count;
    double variance = ((double)sum_sq / count) - (mean * mean);
    double stddev = sqrt(variance);
    
    results[0] = mean;
    results[1] = variance;
    results[2] = stddev;
    results[3] = (double)min;
    results[4] = (double)max;
}

void fill_with_primes(int* arr, size_t count) {
    if (arr == NULL || count == 0) return;
    
    int* current = arr;
    int num = 2;
    size_t found = 0;
    
    while (found < count) {
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
            found++;
        }
        num++;
    }
}

int main() {
    char input[32];
    int size;
    
    printf("Enter number of primes to generate (1-%d): ", MAX_SIZE);
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
    
    int primes[MAX_SIZE];
    fill_with_primes(primes, size);
    
    printf("Generated primes: ");
    int* ptr = primes;
    int* end = primes + size;
    while (ptr < end) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    double stats[5];
    compute_statistics(primes, size, stats);
    
    printf("Statistics:\n");
    printf("Mean: %.2f\n", stats[0]);
    printf("Variance: %.2f\n", stats[1]);
    printf("Standard deviation: %.2f\n", stats[2]);
    printf("Minimum: %.0f\n", stats[3]);
    printf("Maximum: %.0f\n", stats[4]);
    
    return 0;
}