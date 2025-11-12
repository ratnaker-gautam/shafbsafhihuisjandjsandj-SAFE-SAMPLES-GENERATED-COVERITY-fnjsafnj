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
    if (arr == NULL || size < 1 || size > MAX_SIZE) return;
    
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        *ptr = (i * i + 3 * i + 7) % 97;
        ptr++;
    }
}

void compute_stats(int* arr, int size, double* mean, double* stddev) {
    if (arr == NULL || size < 1 || size > MAX_SIZE || mean == NULL || stddev == NULL) return;
    
    double sum = 0.0;
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        ptr++;
    }
    *mean = sum / size;
    
    double sum_sq = 0.0;
    ptr = arr;
    for (int i = 0; i < size; i++) {
        double diff = *ptr - *mean;
        sum_sq += diff * diff;
        ptr++;
    }
    *stddev = sqrt(sum_sq / size);
}

void find_extremes(int* arr, int size, int* min_val, int* max_val, int* min_idx, int* max_idx) {
    if (arr == NULL || size < 1 || size > MAX_SIZE || min_val == NULL || max_val == NULL || min_idx == NULL || max_idx == NULL) return;
    
    *min_val = *arr;
    *max_val = *arr;
    *min_idx = 0;
    *max_idx = 0;
    
    int* ptr = arr + 1;
    for (int i = 1; i < size; i++) {
        if (*ptr < *min_val) {
            *min_val = *ptr;
            *min_idx = i;
        }
        if (*ptr > *max_val) {
            *max_val = *ptr;
            *max_idx = i;
        }
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
    fill_array(arr, size);
    
    printf("Generated array: ");
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    double mean, stddev;
    compute_stats(arr, size, &mean, &stddev);
    printf("Mean: %.2f, Standard deviation: %.2f\n", mean, stddev);
    
    int min_val, max_val, min_idx, max_idx;
    find_extremes(arr, size, &min_val, &max_val, &min_idx, &max_idx);
    printf("Minimum: %d at index %d\n", min_val, min_idx);
    printf("Maximum: %d at index %d\n", max_val, max_idx);
    
    return 0;
}