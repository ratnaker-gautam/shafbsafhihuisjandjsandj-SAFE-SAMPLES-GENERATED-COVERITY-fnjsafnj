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

void compute_statistics(int* data, int size, double* results) {
    if (data == NULL || results == NULL || size <= 0) return;
    
    int* ptr = data;
    double sum = 0.0;
    double sum_sq = 0.0;
    int min = *ptr;
    int max = *ptr;
    
    for (int i = 0; i < size; i++) {
        sum += (double)*ptr;
        sum_sq += (double)(*ptr) * (double)(*ptr);
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        ptr++;
    }
    
    double mean = sum / (double)size;
    double variance = (sum_sq / (double)size) - (mean * mean);
    if (variance < 0) variance = 0;
    double std_dev = sqrt(variance);
    
    results[0] = mean;
    results[1] = std_dev;
    results[2] = (double)min;
    results[3] = (double)max;
}

void fill_with_primes(int* arr, int size) {
    if (arr == NULL || size <= 0) return;
    
    int count = 0;
    int num = 2;
    int* ptr = arr;
    
    while (count < size) {
        int is_prime = 1;
        for (int i = 2; i <= (int)sqrt((double)num); i++) {
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
        if (num < 2) break;
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
    } else {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int data[MAX_SIZE];
    double results[4];
    
    fill_with_primes(data, size);
    compute_statistics(data, size, results);
    
    printf("First %d prime numbers:\n", size);
    int* ptr = data;
    for (int i = 0; i < size; i++) {
        printf("%d ", *ptr);
        ptr++;
        if ((i + 1) % 10 == 0) printf("\n");
    }
    if (size % 10 != 0) printf("\n");
    
    printf("\nStatistics:\n");
    printf("Mean: %.2f\n", results[0]);
    printf("Standard deviation: %.2f\n", results[1]);
    printf("Minimum: %.0f\n", results[2]);
    printf("Maximum: %.0f\n", results[3]);
    
    return 0;
}