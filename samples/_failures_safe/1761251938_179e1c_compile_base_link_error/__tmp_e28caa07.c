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

void compute_statistics(int* arr, size_t n, double* results) {
    if (arr == NULL || results == NULL || n == 0) return;
    
    int* ptr = arr;
    long long sum = 0;
    int min_val = INT_MAX;
    int max_val = INT_MIN;
    
    for (size_t i = 0; i < n; i++) {
        sum += *(ptr + i);
        if (*(ptr + i) < min_val) min_val = *(ptr + i);
        if (*(ptr + i) > max_val) max_val = *(ptr + i);
    }
    
    double mean = (double)sum / n;
    double variance = 0.0;
    
    ptr = arr;
    for (size_t i = 0; i < n; i++) {
        double diff = *(ptr + i) - mean;
        variance += diff * diff;
    }
    variance /= n;
    
    results[0] = mean;
    results[1] = sqrt(variance);
    results[2] = (double)min_val;
    results[3] = (double)max_val;
}

void fill_array(int* arr, size_t n) {
    if (arr == NULL || n == 0) return;
    
    int* ptr = arr;
    for (size_t i = 0; i < n; i++) {
        *(ptr + i) = (int)((i * 17 + 23) % 97);
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
    } else {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    fill_array(arr, size);
    
    printf("Generated array: ");
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");
    
    double results[4];
    compute_statistics(arr, size, results);
    
    printf("Statistics:\n");
    printf("Mean: %.2f\n", results[0]);
    printf("Std Dev: %.2f\n", results[1]);
    printf("Min: %.0f\n", results[2]);
    printf("Max: %.0f\n", results[3]);
    
    return 0;
}