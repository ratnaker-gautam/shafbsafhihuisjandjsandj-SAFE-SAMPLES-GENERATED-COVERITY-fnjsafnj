//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

void fill_fibonacci(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    if (size >= 1) *arr = 0;
    if (size >= 2) *(arr + 1) = 1;
    
    for (size_t i = 2; i < size; i++) {
        long long next = (long long)*(arr + i - 1) + (long long)*(arr + i - 2);
        if (next > INT_MAX) {
            *(arr + i) = 0;
        } else {
            *(arr + i) = (int)next;
        }
    }
}

int main() {
    char input[32];
    int size;
    
    printf("Enter number of Fibonacci elements (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int data[MAX_SIZE];
    fill_fibonacci(data, size);
    
    printf("Fibonacci sequence: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", *(data + i));
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