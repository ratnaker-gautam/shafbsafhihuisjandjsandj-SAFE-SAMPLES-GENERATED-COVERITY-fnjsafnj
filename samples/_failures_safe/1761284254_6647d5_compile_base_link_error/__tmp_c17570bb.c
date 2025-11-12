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
    if (arr == NULL || size <= 0 || size > MAX_SIZE) return;
    
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        *ptr = (i + 1) * (i + 1);
        ptr++;
    }
}

double compute_statistics(int* arr, int size, double* mean, double* stddev) {
    if (arr == NULL || mean == NULL || stddev == NULL || size <= 0 || size > MAX_SIZE) {
        return -1.0;
    }
    
    long long sum = 0;
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        ptr++;
    }
    
    *mean = (double)sum / size;
    
    double variance_sum = 0.0;
    ptr = arr;
    for (int i = 0; i < size; i++) {
        double diff = *ptr - *mean;
        variance_sum += diff * diff;
        ptr++;
    }
    
    *stddev = sqrt(variance_sum / size);
    return 0.0;
}

void print_results(int* arr, int size, double mean, double stddev) {
    if (arr == NULL || size <= 0 || size > MAX_SIZE) return;
    
    printf("Array elements:\n");
    int* ptr = arr;
    for (int i = 0; i < size; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    printf("Mean: %.2f\n", mean);
    printf("Standard deviation: %.2f\n", stddev);
}

int main() {
    char input_buffer[32];
    int size;
    
    printf("Enter array size (1-%d): ", MAX_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    } else if (len == sizeof(input_buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (validate_input(input_buffer, &size) != 0) {
        fprintf(stderr, "Invalid input. Please enter a number between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    int arr[MAX_SIZE];
    fill_array(arr, size);
    
    double mean, stddev;
    if (compute_statistics(arr, size, &mean, &stddev) != 0.0) {
        fprintf(stderr, "Error computing statistics\n");
        return 1;
    }
    
    print_results(arr, size, mean, stddev);
    
    return 0;
}