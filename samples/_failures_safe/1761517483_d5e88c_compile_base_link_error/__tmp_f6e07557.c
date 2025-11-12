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

void fill_array(int* arr, int size) {
    if (arr == NULL || size <= 0) return;
    
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        *(ptr + i) = (i + 1) * (i + 1);
    }
}

double compute_statistics(int* arr, int size, double* mean, double* stddev) {
    if (arr == NULL || size <= 0 || mean == NULL || stddev == NULL) return -1.0;
    
    long long sum = 0;
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        sum += *(ptr + i);
    }
    
    *mean = (double)sum / size;
    
    double variance_sum = 0.0;
    ptr = arr;
    for (int i = 0; i < size; i++) {
        double diff = *(ptr + i) - *mean;
        variance_sum += diff * diff;
    }
    
    *stddev = sqrt(variance_sum / size);
    
    double median;
    if (size % 2 == 0) {
        int mid1 = *(arr + size/2 - 1);
        int mid2 = *(arr + size/2);
        median = (mid1 + mid2) / 2.0;
    } else {
        median = *(arr + size/2);
    }
    
    return median;
}

int main() {
    char input[32];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (validate_input(input, &size) != 0) {
        fprintf(stderr, "Invalid input. Please enter a number between 1 and %d\n", MAX_SIZE);
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
    
    double mean, stddev;
    double median = compute_statistics(arr, size, &mean, &stddev);
    
    if (median < 0.0) {
        fprintf(stderr, "Error computing statistics\n");
        return 1;
    }
    
    printf("Statistics:\n");
    printf("Mean: %.2f\n", mean);
    printf("Median: %.2f\n", median);
    printf("Standard deviation: %.2f\n", stddev);
    
    return 0;
}