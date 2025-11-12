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

void compute_statistics(int* arr, int size, double* results) {
    if (arr == NULL || results == NULL || size <= 0) return;
    
    int* ptr = arr;
    double sum = 0.0;
    double sum_sq = 0.0;
    int min = *ptr;
    int max = *ptr;
    
    for (int i = 0; i < size; i++) {
        int val = *(ptr + i);
        sum += val;
        sum_sq += val * val;
        if (val < min) min = val;
        if (val > max) max = val;
    }
    
    double mean = sum / size;
    double variance = (sum_sq / size) - (mean * mean);
    if (variance < 0) variance = 0;
    double stddev = sqrt(variance);
    
    results[0] = mean;
    results[1] = stddev;
    results[2] = (double)min;
    results[3] = (double)max;
}

int main() {
    char input[32];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid size\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    printf("Enter %d integers:\n", size);
    
    for (int i = 0; i < size; i++) {
        printf("Element %d: ", i + 1);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        
        input[strcspn(input, "\n")] = '\0';
        int value;
        if (validate_input(input, &value) != 0) {
            fprintf(stderr, "Invalid number\n");
            return 1;
        }
        
        *(arr + i) = value;
    }
    
    double results[4];
    compute_statistics(arr, size, results);
    
    printf("\nResults:\n");
    printf("Mean: %.2f\n", results[0]);
    printf("Standard deviation: %.2f\n", results[1]);
    printf("Minimum: %.0f\n", results[2]);
    printf("Maximum: %.0f\n", results[3]);
    
    return 0;
}