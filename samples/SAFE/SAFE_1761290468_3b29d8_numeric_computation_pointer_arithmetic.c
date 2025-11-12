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
    
    int* end = arr + size;
    int* current = arr;
    int value = 1;
    
    while (current < end) {
        *current = value * value;
        value++;
        current++;
    }
}

double compute_statistics(int* arr, int size, double* mean, double* stddev) {
    if (arr == NULL || mean == NULL || stddev == NULL || size < 1) return -1;
    
    long long sum = 0;
    long long sum_squares = 0;
    int* end = arr + size;
    int* current = arr;
    
    while (current < end) {
        sum += *current;
        sum_squares += (long long)(*current) * (long long)(*current);
        current++;
    }
    
    *mean = (double)sum / size;
    
    double variance = ((double)sum_squares / size) - (*mean * *mean);
    if (variance < 0) variance = 0;
    *stddev = sqrt(variance);
    
    return 0;
}

int find_max(int* arr, int size) {
    if (arr == NULL || size < 1) return -1;
    
    int* end = arr + size;
    int* current = arr;
    int max_value = *arr;
    
    while (current < end) {
        if (*current > max_value) {
            max_value = *current;
        }
        current++;
    }
    
    return max_value;
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
    int* end = arr + size;
    int* current = arr;
    while (current < end) {
        printf("%d ", *current);
        current++;
    }
    printf("\n");
    
    double mean, stddev;
    if (compute_statistics(arr, size, &mean, &stddev) == 0) {
        printf("Mean: %.2f\n", mean);
        printf("Standard deviation: %.2f\n", stddev);
    }
    
    int max_val = find_max(arr, size);
    if (max_val != -1) {
        printf("Maximum value: %d\n", max_val);
    }
    
    return 0;
}