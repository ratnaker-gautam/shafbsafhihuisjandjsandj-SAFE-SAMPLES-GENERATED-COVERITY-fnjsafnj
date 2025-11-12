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
        *ptr = (i * i + 3 * i + 7) % 97;
        ptr++;
    }
}

void compute_statistics(int* arr, int size, double* mean, double* stddev) {
    if (arr == NULL || size <= 0 || mean == NULL || stddev == NULL) return;
    
    double sum = 0.0;
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        ptr++;
    }
    *mean = sum / size;
    
    double variance = 0.0;
    ptr = arr;
    for (int i = 0; i < size; i++) {
        double diff = *ptr - *mean;
        variance += diff * diff;
        ptr++;
    }
    variance /= size;
    *stddev = sqrt(variance);
}

void find_extremes(int* arr, int size, int* min_val, int* max_val) {
    if (arr == NULL || size <= 0 || min_val == NULL || max_val == NULL) return;
    
    *min_val = *arr;
    *max_val = *arr;
    
    int* ptr = arr + 1;
    for (int i = 1; i < size; i++) {
        if (*ptr < *min_val) *min_val = *ptr;
        if (*ptr > *max_val) *max_val = *ptr;
        ptr++;
    }
}

int main() {
    char input[32];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else {
        printf("Input too long\n");
        return 1;
    }
    
    if (validate_input(input, &size) != 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    int arr[MAX_SIZE];
    fill_array(arr, size);
    
    printf("Array elements: ");
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    double mean, stddev;
    compute_statistics(arr, size, &mean, &stddev);
    printf("Mean: %.2f\n", mean);
    printf("Standard deviation: %.2f\n", stddev);
    
    int min_val, max_val;
    find_extremes(arr, size, &min_val, &max_val);
    printf("Minimum: %d\n", min_val);
    printf("Maximum: %d\n", max_val);
    
    return 0;
}