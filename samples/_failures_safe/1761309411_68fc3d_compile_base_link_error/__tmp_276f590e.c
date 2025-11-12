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
    
    int sum = 0;
    int min = *arr;
    int max = *arr;
    
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        if (*ptr < min) min = *ptr;
        if (*ptr > max) max = *ptr;
        ptr++;
    }
    
    double mean = (double)sum / size;
    double variance = 0.0;
    
    ptr = arr;
    for (int i = 0; i < size; i++) {
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

void fill_array(int* arr, int size) {
    if (arr == NULL || size <= 0) return;
    
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        *ptr = (i * 7 + 3) % 23;
        ptr++;
    }
}

int main() {
    char input[32];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    double results[4];
    
    fill_array(arr, size);
    compute_statistics(arr, size, results);
    
    printf("Array elements: ");
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    printf("Mean: %.2f\n", results[0]);
    printf("Std Dev: %.2f\n", results[1]);
    printf("Min: %.0f\n", results[2]);
    printf("Max: %.0f\n", results[3]);
    
    return 0;
}