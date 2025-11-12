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
    double sum = 0.0;
    double min = (double)(*ptr);
    double max = (double)(*ptr);
    
    for (size_t i = 0; i < size; i++) {
        double current = (double)(*(ptr + i));
        sum += current;
        if (current < min) min = current;
        if (current > max) max = current;
    }
    
    double mean = sum / (double)size;
    double variance = 0.0;
    
    for (size_t i = 0; i < size; i++) {
        double diff = (double)(*(ptr + i)) - mean;
        variance += diff * diff;
    }
    variance /= (double)size;
    
    results[0] = mean;
    results[1] = sqrt(variance);
    results[2] = min;
    results[3] = max;
}

void fill_fibonacci(int* arr, size_t size) {
    if (arr == NULL || size == 0) return;
    
    int* ptr = arr;
    if (size >= 1) *ptr = 0;
    if (size >= 2) *(ptr + 1) = 1;
    
    for (size_t i = 2; i < size; i++) {
        long long next = (long long)(*(ptr + i - 1)) + (long long)(*(ptr + i - 2));
        if (next > 2147483647LL) {
            *(ptr + i) = 0;
        } else {
            *(ptr + i) = (int)next;
        }
    }
}

int main() {
    char input[32];
    int size;
    
    printf("Enter sequence size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int sequence[MAX_SIZE];
    fill_fibonacci(sequence, (size_t)size);
    
    printf("Fibonacci sequence: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", *(sequence + i));
    }
    printf("\n");
    
    double stats[4];
    compute_statistics(sequence, (size_t)size, stats);
    
    printf("Statistics:\n");
    printf("Mean: %.2f\n", stats[0]);
    printf("Std Dev: %.2f\n", stats[1]);
    printf("Min: %.0f\n", stats[2]);
    printf("Max: %.0f\n", stats[3]);
    
    return 0;
}